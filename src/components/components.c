#include "../../engine.h"


void transform_construct(Transform *const t)
{
    memset(t, 0, sizeof(Transform));
}



void soundSource_construct(SoundSource *const s, char const *const path)
{
	uint8_t channels, bps;
    uint32_t samplerate, size;
    ALvoid *data;

    data = wavFile_load(path, &channels, &samplerate, &bps, &size);

    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, wav_format_get(channels, bps), data, size, samplerate);

    alGenSources(1, &s->source);
    alSourcei(s->source, AL_BUFFER, buffer);

    alDeleteBuffers(1, &buffer);
    free(data);
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


void renderable_construct(Renderable *const r)
{
    
}


void renderable_destruct(Renderable *const r)
{

}


void camera_construct(Camera *const c)
{
    c->zoom = 1;
    c->aspectRatio = 1;
}


void camera_default_zoom(Camera *const c, float const yoffset)
{
    /*the offset has to be between -1 and 1*/
    c->zoom += yoffset * 0.05f;
    c->zoom = fmaxf(c->zoom, 0.05f);
}


void camera_default_resize(Camera *const c, uint16_t const width, uint16_t const height)
{
    glViewport(0, 0, width, height);
    c->aspectRatio = width / (float)height;
}