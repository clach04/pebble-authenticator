/*
TODO

add delete config check if config is different (https://developer.pebble.com/guides/events-and-services/persistent-storage/)
add more entries
change UUID
improve config for vib settings (move into settings)
handle more config settings (move most NUM_SETTINGS usage to use settings count)
consider moving current current_token into settings, con is that it would rewrites all settings on exit (so maybe not)
*/
#include <pebble.h>

#include <pebble-packet/pebble-packet.h>

#include "configuration.h"
#include "sha1.h"
#include "base32.h"


static Window *window=NULL;
static TextLayer *label_layer=NULL;
static TextLayer *token_layer=NULL;
static TextLayer *ticker_layer=NULL;

static int current_token=0;
static bool current_token_changed=false;
static int timezone_mins_offset=0;  // i.e. UTC/GMT-0 only used for Aplite
time_t timeout_timer=0;
time_t timout_period=2 * 60;  // TODO move into settings

typedef struct persist {
    int num_entries;
    char otp_labels[NUM_SECRETS][17];  // labels for otp_keys[]
    unsigned char otp_keys[NUM_SECRETS][10];  // raw bytes for secrets (the integer in a byte array)
    int otp_sizes[NUM_SECRETS];  // number of bytes for otp_keys[] entries
} __attribute__((__packed__)) persist;

persist settings = {
    .num_entries = NUM_SECRETS,
    .otp_labels = {
    	"gtest",
        "fake",
    },
    .otp_keys = {
    	{ 0x66, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F },  // secret (in base32) "MZXW633PN5XW6===" == 'fooooooo' , See https://github.com/google/google-authenticator/issues/70
    	{ 0x7C, 0x94, 0x50, 0xEA, 0xA7, 0x2A, 0x08, 0x66, 0xA3, 0x47 },  // secret (in base32) "PSKFB2VHFIEGNI2H"
    },
    .otp_sizes = {8,10,},
};

void handle_second_tick(struct tm *tick_time, TimeUnits units_changed);

void reset_timeout()
{
    timeout_timer = time(NULL);
}

void set_timezone() {
    int value_read=-1;
    // load config
    if (persist_exists(MESSAGE_KEY_PEBBLE_SETTINGS))
    {
        value_read = persist_read_data(MESSAGE_KEY_PEBBLE_SETTINGS, &settings, sizeof(settings));
        APP_LOG(APP_LOG_LEVEL_DEBUG, "tz settings loaded %d bytes", value_read);
    }
    else
    {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "settings NOT loaded, using defaults");
    }

	if (persist_exists(MESSAGE_KEY_timezone)) {
		timezone_mins_offset = persist_read_int(MESSAGE_KEY_timezone);
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Using timezone minutes offset=%d", timezone_mins_offset);
	}
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
    int data_len=-1;
    unsigned char temp_key[10];
    char * temp_key_base32=NULL;
	Tuple *timezone_tuple = dict_find(iter, MESSAGE_KEY_timezone);
	Tuple *vib_warn_tuple = dict_find(iter, MESSAGE_KEY_vib_warn);
	Tuple *vib_renew_tuple = dict_find(iter, MESSAGE_KEY_vib_renew);

    reset_timeout();
	if (timezone_tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "incoming message timezone");
		persist_write_int(MESSAGE_KEY_timezone, timezone_tuple->value->int32);
		set_timezone();
	}
	if (vib_warn_tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "incoming message vib_warn");
		persist_write_bool(MESSAGE_KEY_vib_warn, vib_warn_tuple->value->uint8);
	}
	if (vib_renew_tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "incoming message vib_renew");
		persist_write_bool(MESSAGE_KEY_vib_renew, vib_renew_tuple->value->uint8);
	}

    if(packet_contains_key(iter, MESSAGE_KEY_S00_NAME))
    {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "incoming message MESSAGE_KEY_S00_NAME");
        strncpy(settings.otp_labels[0], packet_get_string(iter, MESSAGE_KEY_S00_NAME), sizeof(settings.otp_labels[0])-1);
        // TODO persist it
        // need data length...
        //memcpy(settings.otp_keys[0], packet_get_string(iter, MESSAGE_KEY_S00_NAME), sizeof(settings.otp_keys[0]));  // for later
    }
    if(packet_contains_key(iter, MESSAGE_KEY_S00_SECRET))
    {
        #define min(x, y)  x < y ? x : y
        APP_LOG(APP_LOG_LEVEL_DEBUG, "incoming message MESSAGE_KEY_S00_SECRET");
        temp_key_base32 = packet_get_string(iter, MESSAGE_KEY_S00_SECRET);
        data_len = base32_decode((uint8_t *) temp_key_base32, temp_key, strlen(temp_key_base32)); // potential for buffer overrun? certainly potential for error
        // check data_len for errors before copying?
        APP_LOG(APP_LOG_LEVEL_INFO, "MESSAGE_KEY_S00_SECRET (after b32) len %d", (int) data_len);
        memcpy(settings.otp_keys[0], temp_key, min((unsigned int) data_len, sizeof(settings.otp_keys[0])));
        settings.otp_sizes[0] = data_len;

        // TODO persist it

        // Force screen refresh (on next second)
        current_token_changed = true;
    }
    if (current_token_changed)
    {
        int value_written=-1;

        value_written = persist_write_data(MESSAGE_KEY_PEBBLE_SETTINGS, &settings, sizeof(settings));
        APP_LOG(APP_LOG_LEVEL_DEBUG, "write settings: %d", value_written);
        if (value_written >= 0)  // TODO compare with sizeof()?
        {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "write settings SUCCESS");
        }
        else
        {
            APP_LOG(APP_LOG_LEVEL_DEBUG, "write settings FAILURE");
        }
    }
}

void in_dropped_handler(AppMessageResult reason, void *context) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "incoming message from Pebble dropped");
}

void vibration_handler(int current_seconds) {
	if (current_seconds == 5 && persist_exists(MESSAGE_KEY_vib_warn) && persist_read_bool(MESSAGE_KEY_vib_warn)) {
		vibes_double_pulse();
	}
	if (current_seconds == 30 && persist_exists(MESSAGE_KEY_vib_renew) && persist_read_bool(MESSAGE_KEY_vib_renew)) {
		vibes_short_pulse();
	}
}

uint32_t get_token() {
	sha1nfo s;
	uint8_t ofs;
	uint32_t otp;
	char sha1_time[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

	// TOTP uses seconds since epoch in the upper half of an 8 byte payload
	// TOTP is HOTP with a time based payload
	// HOTP is HMAC with a truncation function to get a short decimal key
	uint32_t unix_time = time(NULL);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "raw unix_time()=%lu", unix_time);
#ifdef PBL_PLATFORM_APLITE
	// firmware 3 is supposed to be available for Aplite but in CloudPebble this is locale and not UTC
	int adjustment = 60 * -1 * timezone_mins_offset;

	unix_time = unix_time - adjustment;
#endif  // else Firmware 3+ basalt, chalk and later so is UTC already
	APP_LOG(APP_LOG_LEVEL_DEBUG, "UTC unix_time()=%lu", unix_time);
	unix_time /= 30;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "UTC unix_time mod 30=%lu", unix_time);

	sha1_time[4] = (unix_time >> 24) & 0xFF;
	sha1_time[5] = (unix_time >> 16) & 0xFF;
	sha1_time[6] = (unix_time >> 8) & 0xFF;
	sha1_time[7] = unix_time & 0xFF;

	// First get the HMAC hash of the time payload with the shared key
	sha1_initHmac(&s, settings.otp_keys[current_token], settings.otp_sizes[current_token]);
	sha1_write(&s, sha1_time, 8);
	sha1_resultHmac(&s);

	// Then do the HOTP truncation. HOTP pulls its result from a 31-bit byte
	// aligned window in the HMAC result, then lops off digits to the left
	// over 6 digits.
	ofs = s.state.b[HASH_LENGTH-1] & 0xf;
	otp = 0;
	otp = ((s.state.b[ofs] & 0x7f) << 24) |
		((s.state.b[ofs + 1] & 0xff) << 16) |
		((s.state.b[ofs + 2] & 0xff) << 8) |
		(s.state.b[ofs + 3] & 0xff);
	otp %= 1000000;

	return otp;
}

void handle_second_tick(struct tm *tick_time, TimeUnits units_changed) {
	int current_seconds = 30 - (tick_time->tm_sec % 30);

    if (timout_period != 0)
    {
        if (time(NULL) - timeout_timer >= timout_period)
        {
            // From https://forums.pebble.com/t/solved-proper-watch-app-exit-method/9976
            // https://developer.pebble.com/docs/c/User_Interface/Window_Stack/#window_stack_pop_all
            window_stack_pop_all(true);
        }
    }

	vibration_handler(current_seconds);

	if (current_token_changed || current_seconds == 30) {
		current_token_changed = false;

		static char token_text[] = "000:000";  // extra character to break up spacing
		uint32_t token = get_token();
		snprintf(token_text, sizeof(token_text), "%03lu-%03lu", token / 1000, token % 1000);  // BITHAM_34_MEDIUM_NUMBERS has '-' but missing space
		//snprintf(token_text, sizeof(token_text), "%06lu", get_token());
        //APP_LOG(APP_LOG_LEVEL_DEBUG, "token num %06lu", token);
        //APP_LOG(APP_LOG_LEVEL_DEBUG, "token_text %s", token_text);

		text_layer_set_text(label_layer, settings.otp_labels[current_token]);
		text_layer_set_text(token_layer, token_text);
	}

	static char ticker_text[] = "00";
	snprintf(ticker_text, sizeof(ticker_text), "%d", current_seconds);
	text_layer_set_text(ticker_layer, ticker_text);
}

static void click_handler(ClickRecognizerRef recognizer, Window *window) {
	switch ((int)click_recognizer_get_button_id(recognizer)) {
		case BUTTON_ID_UP:
			current_token = (current_token - 1 + NUM_SECRETS) % NUM_SECRETS;
			current_token_changed = true;
			break;
		case BUTTON_ID_DOWN:
			current_token = (current_token + 1) % NUM_SECRETS;
			current_token_changed = true;
			break;
	}
    reset_timeout();
	time_t now = time(NULL);
	handle_second_tick(gmtime(&now), SECOND_UNIT);
}

static void click_config_provider(void *context) {
	const uint16_t repeat_interval_ms = 100;
	window_single_repeating_click_subscribe(BUTTON_ID_UP, repeat_interval_ms, (ClickHandler) click_handler);
	window_single_repeating_click_subscribe(BUTTON_ID_DOWN, repeat_interval_ms, (ClickHandler) click_handler);
}

static void window_load(Window *window) {
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);

	label_layer = text_layer_create((GRect) { .origin = { 0, 20 }, .size = bounds.size });
	text_layer_set_text_color(label_layer, GColorWhite);
	text_layer_set_background_color(label_layer, GColorClear);
	text_layer_set_font(label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(label_layer, GTextAlignmentCenter);

	token_layer = text_layer_create((GRect) { .origin = { 0, 60 }, .size = bounds.size });
	text_layer_set_text_color(token_layer, GColorWhite);
	text_layer_set_background_color(token_layer, GColorClear);
	text_layer_set_font(token_layer, fonts_get_system_font(FONT_KEY_BITHAM_34_MEDIUM_NUMBERS));
	text_layer_set_text_alignment(token_layer, GTextAlignmentCenter);

	ticker_layer = text_layer_create((GRect) { .origin = { 0, 120 }, .size = bounds.size });
	text_layer_set_text_color(ticker_layer, GColorWhite);
	text_layer_set_background_color(ticker_layer, GColorClear);
	text_layer_set_font(ticker_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
	text_layer_set_text_alignment(ticker_layer, GTextAlignmentCenter);

	layer_add_child(window_layer, text_layer_get_layer(label_layer));
	layer_add_child(window_layer, text_layer_get_layer(token_layer));
	layer_add_child(window_layer, text_layer_get_layer(ticker_layer));

	tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);
}

static void window_unload(Window *window) {
	tick_timer_service_unsubscribe();
	text_layer_destroy(label_layer);
	text_layer_destroy(token_layer);
	text_layer_destroy(ticker_layer);
}

static void app_message_init(void) {
	app_message_open(64 /* inbound_size */, 0 /* outbound_size */);
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
}

static void init(void) {
	app_message_init();

	current_token = persist_exists(MESSAGE_KEY_CURRENT_TOKEN) ? persist_read_int(MESSAGE_KEY_CURRENT_TOKEN) : 0;
	current_token_changed = true;

	window = window_create();
	window_set_click_config_provider(window, click_config_provider);
	window_set_window_handlers(window, (WindowHandlers) {
		.load = window_load,
		.unload = window_unload,
	});
	window_stack_push(window, true /* animated */);
	window_set_background_color(window, GColorBlack);

	set_timezone();
    reset_timeout();

	time_t now = time(NULL);
	handle_second_tick(gmtime(&now), SECOND_UNIT);
}

static void deinit(void) {
	persist_write_int(MESSAGE_KEY_CURRENT_TOKEN, current_token);
	window_destroy(window);
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
