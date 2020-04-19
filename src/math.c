#include "../utility.h"


static uint16_t sr;
static uint8_t bit;


uint8_t limitUint8(uint8_t const val, uint8_t const min, uint8_t const max)
{
    return val < min ? min : val > max ? max : val;
}

float limitF(float const val, float const min, float const max)
{
    return val < min ? min : val > max ? max : val;
}


void sr_init(void)
{
    srand(time(0));
    sr = rand();
}


uint16_t getRandUint16(uint16_t const min, uint16_t const max)
{
    bit  = (sr) ^ (sr >> 2) ^ (sr >> 3) ^ (sr >> 5);
    sr =  (sr >> 1) | (bit << 15);
    //normalize sr between 0 and 1 -> sr / uint16Range
    //multiply by max number to get number in range 0 - max
    //add 0.5 and cast implicitly to int to round to nearest intenger
    return (float)sr / USHRT_MAX * (max - min) + 0.5f + min;
}


uint8_t getRandUint8(uint8_t const min, uint8_t const max)
{
    return getRandUint16(min, max);
}


bool getRandBool(float const chance)
{
    return getRandUint16(0, USHRT_MAX) <= USHRT_MAX * chance;
}


int mround(int const number, int const multiple)
{
    return ((number + multiple / 2) / multiple) * multiple;
}


float getRandF(float min, float max)
{
    return (((float)rand()) / (float) RAND_MAX * (max - min)) + min;
}