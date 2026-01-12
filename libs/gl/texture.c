#include "gl.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_SIMD
#define STBI_FAILURE_USERMSG
#include "vendor/stb/stb_image.h"


Image images[16];
uint8_t image_count;

//an image is a buffer of colors that can be load from a file
//a texture is a index into an image to indicate where the texture starts and ends
void textureManager_init(void)
{

}


void textureManager_terminate(void)
{
	for (uint8_t i=0; i<image_count; ++i)
		glDeleteTextures(1, &images[i].id);
}


//has to be called before texture_construct
//loads and binds rtexture to create textures from
Image textureManager_image_load(char const *const path)
{
	Image image;
	glGenTextures(1, &image.id);
	images[image_count++] = image;

	glBindTexture(GL_TEXTURE_2D, image.id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //makes the image sharper instead of interpolating
	int width, height, channels;
	void *buffer = stbi_load(path, &width, &height, &channels, 4);
	if (!buffer)//4 because RGBA
	{
		printf("Failed to load image file %s\n", path);
		buffer = stbi_load("./resources/error/errorTexture.png", &width, &height, &channels, 4); //4 because RGBA
		assert(buffer);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	stbi_image_free(buffer);

	image.width = width;
	image.height = height;
	return image;
}


void texture_construct(Texture *const t, uint16_t const x, uint16_t const y, uint16_t const width, uint16_t const height, Image* image)
{
	*t = (Texture)
	{
		.pos = {x / (float)image->width, y / (float)image->height}, 
		.size = {width / (float)image->width, height / (float)image->height}, 
		.image = image
	};
}
