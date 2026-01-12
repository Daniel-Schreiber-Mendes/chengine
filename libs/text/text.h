#ifndef TEXT_H
#define TEXT_H
#include <ft2build.h>
#include FT_FREETYPE_H 
#include <stdint.h>
#include <GL/glew.h>
#include <math.h>
#include <gl/gl.h>
#include <cglm/cglm.h>


typedef struct
{ 
	uint16_t width, height, bearing_x, bearing_y;
	uint32_t advance;
	uint32_t texture;
}												
Character;



typedef struct
{
	FT_Face face;
	Character chars[128];
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
void font_load(Font *f, char const *path, uint8_t size);
void text_render(Font *f);


#endif