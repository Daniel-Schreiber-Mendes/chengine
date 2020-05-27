#include "audio.h"

static ALCdevice *device;
static ALCcontext *context;


void audio_init(void)
{
	/* device is just a soundcard*/
	device = alcOpenDevice(NULL);
	che_assert(device);
	context = alcCreateContext(device, NULL);
	che_assert(context);
	alcMakeContextCurrent(context);
}


void audio_terminate(void)
{
	alcCloseDevice(device);
	alcDestroyContext(context);
}
