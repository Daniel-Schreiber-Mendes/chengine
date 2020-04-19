#ifndef UTILITY_H
#define UTILITY_H
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>
#include <GL/glew.h>
#include <stdbool.h>


typedef struct 
{
    uint16_t val, min, max;
    int16_t d;
}
Alternator;


//utility.c
uint8_t  alternator_alternate(Alternator *const alt);
uint8_t  getGlTypeSize(GLuint const type);

//math.c
#define rand_element(arr, size) (arr[getRandUint16(0, size - 1)])
void     sr_init(void);
uint8_t  limitUint8(uint8_t const val, uint8_t const min, uint8_t const max);
float    limitF(float const val, float const min, float const max);
uint16_t getRandUint16(uint16_t const min, uint16_t const max);
uint8_t  getRandUint8(uint8_t const min, uint8_t const max);
bool     getRandBool(float const chance);
int      mround(int const number, int const multiple); //round to multiple of a a given number
float    getRandF(float min, float max);


#endif