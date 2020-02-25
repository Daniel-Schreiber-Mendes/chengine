#ifndef AUDIO_H
#define AUDIO_H
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>


bool is_little_endian(void); //returns if the machine stores the bits in gib endian or little endian order
uint16_t convert_byte2_buffer_to_uint16(uint8_t const *const buffer);

void wavFile_load(char const *const path, uint8_t *channels, uint32_t *const sampleRate);

#endif