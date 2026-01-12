#include "text.h"


static FT_Library ft;
static unsigned int vao, vbo;
static uint32_t program;

void text_init(void)
{
	assert(!FT_Init_FreeType(&ft));

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); 

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 

	program_construct(&program, "libs/text/vertex.glsl", "libs/text/fragment.glsl");
	program_uniform1i_set(program, "font_texture", 0);
}


void text_terminate(void)
{
	FT_Done_FreeType(ft);
	glDeleteProgram(program);
}


void font_load(Font *const f, char const *const path, uint8_t const size)
{
	FT_Face face;
	if (FT_New_Face(ft, path, 0, &face))
	{
		printf("Failed to load font %s\n", path);
		assert(false);
	}

	FT_Set_Pixel_Sizes(face, 0, size); 
	glActiveTexture(GL_TEXTURE0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

	for (uint8_t i=0; i < 128; ++i)
	{
		assert(FT_Load_Char(f->face, i, FT_LOAD_RENDER));

		f->chars[i] = (Character)
	    {
	        .width = face->glyph->bitmap.width, 
	        .height = face->glyph->bitmap.rows,
	        .bearing_x = face->glyph->bitmap_left, 
	        .bearing_y = face->glyph->bitmap_top,
	        .advance = face->glyph->advance.x
	    };

		glGenTextures(1, &f->chars[i].texture);
		glBindTexture(GL_TEXTURE_2D, f->chars[i].texture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, 
			face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	FT_Done_Face(face);
}


void text_render(Font *f)
{
	glUseProgram(program);

	glActiveTexture(GL_TEXTURE1);
	glBindVertexArray(vao);


    Character ch = f->chars[10];

    float xpos = 100;
    float ypos = 100;

    float w = 50;
    float h = 50;
    // update VBO for each character
    float vertices[6][4] = {
        { xpos,     ypos + h,   0.0f, 0.0f },            
        { xpos,     ypos,       0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 1.0f },

        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos + w, ypos,       1.0f, 1.0f },
        { xpos + w, ypos + h,   1.0f, 0.0f }           
    };
    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, 1);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}