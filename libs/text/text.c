#include "text.h"

/*
static FT_Library ft;


void text_init(void)
{
	if (FT_Init_FreeType(&ft))
	{
		printf("Failed to initialize FreeType\n");
	}
}


void text_terminate(void)
{
	FT_Done_FreeType(ft);
}


void font_construct(Font *const f, char const *const path, uint8_t const size)
{
	if (FT_New_Face(ft, path, 0, &f->face))
	{
		printf("Failed to load font %s\n", path);
	}
	FT_Set_Pixel_Sizes(f->face, 0, size); 
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

	uint8_t max_width, max_height;

	for (uint8_t i=0; i < 128; ++i)
	{
		FT_Load_Char(f->face, i, FT_LOAD_RENDER);
	    max_width = fmaxf(max_width, f->face->glyph->bitmap.width);
	    max_height = fmaxf(max_height, f->face->glyph->bitmap.rows);

	    f->characters[i] = (Character)
	    {
	        {f->face->glyph->bitmap.width, f->face->glyph->bitmap.rows},
	        {f->face->glyph->bitmap_left, f->face->glyph->bitmap_top},
	        f->face->glyph->advance.x
	    };
	}

	rtexture_construct(&f->texture, GL_TEXTURE_2D_ARRAY);
	rtexture_bind(&f->texture);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RED, max_width, max_height, 128, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);

	for (uint8_t i=0; i < 128; ++i)
	{
		FT_Load_Char(f->face, i, FT_LOAD_RENDER);
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, f->characters[i].size[0], f->characters[i].size[1], 1, GL_RED, GL_UNSIGNED_BYTE, f->face->glyph->bitmap.buffer);
	}

	f->textureSize[0] = max_width;
	f->textureSize[1] = max_height;
}


void font_destruct(Font *f)
{
	FT_Done_Face(f->face);
}*/