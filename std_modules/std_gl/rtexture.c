#include "gl.h"



void rtexture_construct(RTexture *const rt, GLenum const type)
{
	glGenTextures(1, &rt->id);
	rt->type = type;
}


void rtexture_destruct(RTexture *const rt)
{
	glDeleteTextures(1, &rt->id);
}


void rtexture_bind(RTexture const *const rt)
{
	glBindTexture(rt->type, rt->id);
}