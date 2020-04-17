#ifndef UTILITY_H
#define UTILITY_H
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include <GL/glew.h>
#include <stdbool.h>


typedef struct 
{
    uint8_t val, min, max;
    int16_t d;
}
Alternator;


//utility.c
uint8_t  getRandElement(uint8_t const *const array, uint8_t const size);
uint8_t  alternator_alternate(Alternator *const alt);
uint8_t  getGlTypeSize(GLuint const type);

//math.c
uint16_t sr_init(void);
uint8_t  limitUint8(uint8_t const val, uint8_t const min, uint8_t const max);
float    limitF(float const val, float const min, float const max);
uint16_t getRandUint16(uint16_t const min, uint16_t const max);
uint8_t  getRandUint8(uint8_t const min, uint8_t const max);
bool     getRandBool(float const chance);
int      roundToMultiple(int const number, int const multiple);
float    getRandF(float min, float max);


#endif