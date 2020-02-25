#include "../../audio.h"

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


void wavFile_load(char const *const path, uint8_t *const channels, uint32_t *const sampleRate)
{
	uint8_t buffer[4];

	FILE *file = fopen(path, "rb"); //r for read and b for binary
	assert(file);


	/*first 4 bytes in the wav header is chunk ID and have to be "RIFF". we check this to make sure the file has the right format.
	The RIFF format acts as a "wrapper" for various audio coding formats. */
	fread(buffer, sizeof(uint8_t) * 4, sizeof(uint8_t), file);
	assert(memcmp(buffer, "RIFF", sizeof(uint8_t) * 4) == 0); /*if return value is zero that means the memory is the same*/


	/*next 4 bytes specify the size of the whole header and are called chunk size. since we dont't care what the size is we can just move the file 
	pointer by 4 bytes forward*/
	fseek(file, 4, SEEK_CUR);


	/*the next 4 bytes specify the format. they should be "WAVE".*/
	fread(buffer, sizeof(uint8_t) * 4, sizeof(uint8_t), file);
	assert(memcmp(buffer, "WAVE", sizeof(uint8_t) * 4) == 0);/*"WAVE" specifies that the format we want is wave. if the format is something else, assert.


	/*this subchunk describes the audio's format. it begins with 4 bytes which should be "fmt "*/
	fread(buffer, sizeof(uint8_t) * 4, sizeof(uint8_t), file);
	assert(memcmp(buffer, "fmt ", sizeof(uint8_t) * 4) == 0);


	/* this should contain the size of this subchunk. it should always be 16. because the number 16 is stored in the first byte, we compare
	the first element in the buffer with 16*/
	fread(buffer, sizeof(uint8_t) * 4, sizeof(uint8_t), file);
	assert(buffer[0] == 16);


	/*advance 2 bytes. these two bytes specify if the wav file is compressed in some way. if it is not compressed, the first byte should be 1*/
	fread(buffer, sizeof(uint8_t) * 2, sizeof(uint8_t), file);
	assert(buffer[0] == 1);


	/* the number of channels specify if sound is played e.g mono(1 channel) or stereo (2 channels)*/
	fread(channels, sizeof(uint8_t) * 2, sizeof(uint8_t), file);


	/*the samplerate of the audio data*/
	fread(sampleRate, sizeof(uint8_t) * 4, sizeof(uint8_t), file);


	fclose(file);

}