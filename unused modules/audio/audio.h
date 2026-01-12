#ifndef SOUND_EXT_H
#define SOUND_EXT_h
#include <chengine-dev/engine.h>
#include <AL/al.h>
#include <AL/alc.h>


typedef struct
{
	ALuint source;
}
SoundSource;


void soundSource_construct(SoundSource *const s, char const *const path);
void soundSource_destruct(SoundSource *const s);
void soundSource_sound_play(SoundSource *const s);
void soundSource_position_set(SoundSource *const s, vec3 const pos);

void audio_import(void);
void audio_init(void);
void audio_terminate(void);

bool     is_little_endian(void); //returns if the machine stores the bits in big endian or little endian order
uint16_t convert_byte2_buffer_to_uint16(uint8_t const *const buffer);
void*    wavFile_load(char const *const path, uint8_t *channels, uint32_t *const sampleRate, uint8_t *const bps, uint32_t *const size);
uint16_t wav_format_get(uint8_t const channels, uint8_t const bps);


#endif