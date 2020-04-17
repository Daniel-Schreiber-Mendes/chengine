#include "../../utility.h"


uint8_t getRandElement(uint8_t const *const array, uint8_t const size)
{
    return array[getRandUint16(0, size - 1)];
}


uint8_t alternator_alternate(Alternator *const alt)
{
    if (alt->val == alt->max || alt->val == alt->min)
    {
        alt->d *= -1;
    }
    return alt->val += alt->d;
}


uint8_t typeGL_size_get(GLuint const type)
{
	switch (type)
	{
		case GL_FLOAT: 		  return sizeof(GLfloat);
		case GL_UNSIGNED_INT: return sizeof(GLuint);
	}
	assert(false);
}