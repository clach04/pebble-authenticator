// FIXME static and generated for debug by configuration.py update to use Clay
#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#define DEFAULT_TIME_ZONE +1
#define NUM_SECRETS 1

char otp_labels[NUM_SECRETS][17] = {
	"fake",
};

unsigned char otp_keys[NUM_SECRETS][10] = {
	{ 0x7C, 0x94, 0x50, 0xEA, 0xA7, 0x2A, 0x08, 0x66, 0xA3, 0x47 },
};

int otp_sizes[NUM_SECRETS] = {10,};

#endif
