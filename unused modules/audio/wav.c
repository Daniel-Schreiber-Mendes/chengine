#include "audio.h"


//according to wav standard all bits in a wav file are stored in little endian order
//big endian: most significant bit on the left
//little endian: most significant bit on the right


bool is_little_endian(void)
{
	uint16_t const a = 1;
	/*
	1 in big endian
	00000000 00000001
	 = 0        = 1  if interpreted as uint8_t's
	1 in little endian
	10000000 00000000
	 = 1        = 0  if interpreted as uint8_t's
	*/
	return ((uint8_t*)&a)[0]; //this will index the leftmost byte because the number is interpreted as an array
}


uint16_t convert_byte2_buffer_to_uint16(uint8_t const *const buffer)
{
	if (is_little_endian())
	{
		uint16_t a = 0;
		((uint8_t*)&a)[0] = buffer[1]; //if is little endian reverse the 2 bytes and else just copy the 2 bytes of the buffer into the retval
		((uint8_t*)&a)[1] = buffer[0];
		return a;
	}
	return ((uint16_t*)buffer)[0];
}


void* wavFile_load(char const *const path, uint8_t *const channels, uint32_t *const sampleRate, uint8_t *const bps, uint32_t *const size)
{
	uint8_t buffer[4];

	FILE *file = fopen(path, "rb"); //r for read and b for binary
	che_assert(file);


	/*first 4 bytes in the wav header is chunk ID and have to be "RIFF". we check this to make sure the file has the right format.
	The RIFF format acts as a "wrapper" for various audio coding formats. */
	fread(buffer, 4, 1, file);
	che_assert(memcmp(buffer, "RIFF", 4) == 0); /*if return value is zero that means the memory is the same*/


	/*next 4 bytes specify the size of the whole header and are called chunk size. since we dont't care what the header size is we can just move 
	the file pointer by 4 bytes forward*/
	fseek(file, 4, SEEK_CUR);


	/*the next 4 bytes specify the format. they should be "WAVE".*/
	fread(buffer, 4, 1, file);
	che_assert(memcmp(buffer, "WAVE", 4) == 0);/*"WAVE" specifies that the format we want is wave. if the format is something else, che_assert.*/
	/*this subchunk describes the audio's format. it begins with 4 bytes which should be "fmt "*/
	fread(buffer, 4, 1, file);
	che_assert(memcmp(buffer, "fmt ", 4) == 0);


	/* this should contain the size of this subchunk. it should always be 16. because the number 16 is stored in the first byte, we compare
	the first element in the buffer with 16*/
	fread(buffer, 4, 1, file);
	che_assert(buffer[0] == 16);


	/*advance 2 bytes. these two bytes specify if the wav file is compressed in some way. if it is not compressed, the first byte should be 1*/
	fread(buffer, 2, 1, file);
	che_assert(buffer[0] == 1);


	/* the number of channels specify if sound is played e.g mono(1 channel) or stereo (2 channels)*/
	fread(channels, 2, 1, file);


	/*the samplerate of the audio data*/
	fread(sampleRate, 4, 1, file);


	/*these 4 bytes specify the byterate. we don't care about it so we just move the pointer 4 bytes forward*/
	fseek(file, 4, SEEK_CUR);


	/*these 2 bytes are called blockalign. it specifys the number of bytes per sample on all channels. we don't care about it so we 
	just move the pointer 4 bytes forward*/
	fseek(file, 2, SEEK_CUR);


	/*bits per sample*/
	fread(bps, 2, 1, file);


	/*now the data subchunk begins. to make sure everything is correct we check if the next 4 bytes contain the letters "data".*/
	fread(buffer, 4, 1, file);
	//che_assert(memcmp(buffer, "data", 4) == 0);


	/*this contains the size of the data*/
	fread(size, 4, 1, file);


	/*copys the data out of the file into the data buffer*/
	void *data = che_malloc(*size);
	che_assert(data);
	fread(data, *size, 1, file);


	fclose(file);
	return data;
}

uint16_t wav_format_get(uint8_t const channels, uint8_t const bps)
{
	/*
	the possible formats are defined as the following:
	#define AL_FORMAT_MONO8                          0x1100
	#define AL_FORMAT_MONO16                         0x1101
	#define AL_FORMAT_STEREO8                        0x1102
	#define AL_FORMAT_STEREO16                       0x1103
	bps is either 8 or 16.
	channels is either 1 or 2. the following formula gets the right format out of the possible inputs
	if bps is 8 the offset has to be 0. if it is 16 the offset has to be 2.
	8 -> 0
	16 -> 1
	bps / 8 - 1 produces the desired result
	if channels is 1 the offset has to be 0. if it is 2 the offset has to be 2
	1 -> 0
	2 -> 2
	(channels - 1) * channels fits the requirements
	*/
	return 0x1100 + bps / 8 - 1 + (channels - 1) * channels;
}