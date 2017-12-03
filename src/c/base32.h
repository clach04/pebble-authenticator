#pragma once

#include <stdint.h>
typedef unsigned char uint8_t;  // for Pebble

int base32_decode(const uint8_t *encoded, uint8_t *result, int bufSize)
    __attribute__((visibility("hidden")));
int base32_encode(const uint8_t *data, int length, uint8_t *result,
                  int bufSize)
    __attribute__((visibility("hidden")));
