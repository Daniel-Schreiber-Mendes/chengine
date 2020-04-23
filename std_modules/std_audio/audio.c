#include "audio.h"

static ALCdevice *device;


void audio_init(void)
{
	/* device is just a soundcard*/
	device = alcOpenDevice(NULL);
	che_assert(device);
	ALCcontext* context = alcCreateContext(device, NULL);
	alcMakeContextCurrent(context);
}


void audio_terminate(void)
{
	alcCloseDevice(device);
}
