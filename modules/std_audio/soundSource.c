#include "audio.h"


void soundSource_construct(SoundSource *const s, char const *const path)
{
	uint8_t channels, bps;
    uint32_t samplerate, size;
    ALvoid *const data = wavFile_load(path, &channels, &samplerate, &bps, &size);

    
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, wav_format_get(channels, bps), data, size, samplerate);
    alGenSources(1, &s->source);
    alSourcei(s->source, AL_BUFFER, buffer);
    alDeleteBuffers(1, &buffer);
    
    
    che_scope_free(data);
}


void soundSource_destruct(SoundSource *const s)
{
	alDeleteSources(1, &s->source);
}


void soundSource_sound_play(SoundSource *const s)
{
	alSourcePlay(s->source);
}


void soundSource_position_set(SoundSource *const s, vec3 const pos)
{
    alSourcefv(s->source, AL_POSITION, pos);
}
