#ifndef TEXT_H
#define TEXT_H
#include <ft2build.h>
#include FT_FREETYPE_H 
#include <stdint.h>
#include <GL/glew.h>
#include <math.h>


typedef struct
{
	uint16_t width, height, bearing_x, bearing_y;
	uint32_t advance; // Offset to advance to next glyph
}
Character;


typedef struct
{
	uint32_t texture_id;
	FT_Face face;
	Character characters[128];
}
Font;



void text_init(void);
void text_terminate(void);
void font_construct(Font *f, char const *path, uint8_t size);
void font_destruct(Font *f);


#endif