// FIXME static and generated for debug by configuration.py update to use Clay
#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#define DEFAULT_TIME_ZONE +1
#define NUM_SECRETS 2

char otp_labels[NUM_SECRETS][17] = {  // labels for otp_keys[]
	"gtest",
    "fake",
};

unsigned char otp_keys[NUM_SECRETS][10] = {
	{ 0x66, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F, 0x6F },  // secret (in base32) "MZXW633PN5XW6===" == 'fooooooo' , See https://github.com/google/google-authenticator/issues/70
	{ 0x7C, 0x94, 0x50, 0xEA, 0xA7, 0x2A, 0x08, 0x66, 0xA3, 0x47 },  // secret (in base32) "PSKFB2VHFIEGNI2H"
};

int otp_sizes[NUM_SECRETS] = {8,10,};  // number of bytes for otp_keys[] entries

#endif
