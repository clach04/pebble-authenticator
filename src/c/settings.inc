
// potentially machine generated

#define NUM_SECRETS 5

int config_version=3; // Increment if persist settings changes structure
typedef struct persist {
    int num_entries;
    char otp_labels[NUM_SECRETS][16+1];  // labels for otp_keys[]
    unsigned char otp_keys[NUM_SECRETS][10];  // raw bytes for secrets (integer in a byte array), max 80-bits
    int otp_sizes[NUM_SECRETS];  // number of bytes for otp_keys[] entries
    int time_out_period;
    bool vib_warn;
    bool vib_renew;
    // if add items here or NUM_SECRETS changes, bump "config_version"
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
    .otp_sizes = {8,10,0},  // c99 defaults the rest to zero
    .time_out_period = 2 * 60,  // 2 minutes
    .vib_warn = false,
    .vib_renew = false,
};

// potentially machine generated
// relies on macros in parent C file that includes this
#define SETTINGS_NAME_VALUE_MACRO\
    DO_SETTINGS_NAME(0)\
    DO_SETTINGS_SECRET(0)\
    DO_SETTINGS_NAME(1)\
    DO_SETTINGS_SECRET(1)\
    DO_SETTINGS_NAME(2)\
    DO_SETTINGS_SECRET(2)\
    DO_SETTINGS_NAME(3)\
    DO_SETTINGS_SECRET(3)\
    DO_SETTINGS_NAME(4)\
    DO_SETTINGS_SECRET(4)
// FIXME consider making this a C function?
