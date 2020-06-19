#ifndef TEXT_H
#define TEXT_H
#include <ft2build.h>
#include FT_FREETYPE_H 
#include <stdint.h>
#include <GL/glew.h>
#include <math.h>
#include "../std_gl/gl.h"


typedef struct
{ 
	uint16_t size[2], bearing[2];
	uint32_t advance; //32 and not 16 because one can send only uint32's to the gpu
}												// Offset to advance to next glyph
Character;



typedef struct
{
	FT_Face face;
	RTexture texture;
	Character characters[128];
	uint8_t index; //index into array of loaded fonts
	uint32_t textureSize[2];
}
Font;


typedef struct
{
	Font const *font;
	char const *string;
	uint8_t scale;
}
Text;



void text_init(void);
void text_terminate(void);
void font_construct(Font *f, char const *path, uint8_t size);
void font_destruct(Font *f);


#endif