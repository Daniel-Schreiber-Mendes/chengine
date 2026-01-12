#ifndef ANIMATION_H
#define ANIMATION_H
#include "../render/render.h"
//#include <boost/preprocessor/seq/for_each_i.hpp>


typedef struct
{
	Texture **frames;
	uint8_t frameCount;
}
AnimationPattern;


typedef struct
{
	uint8_t frameDelay, frame; //frameCount is the number of keyframes, framedelay is the number of frames one keyframe is shown 
	AnimationPattern *pattern;
}
Animation; //Component




void animation_import(void);
void animation_task(void);
void animationPattern_construct(AnimationPattern *a, uint8_t frameCount, Texture **t);
void animationPattern_destruct(AnimationPattern *a);


#endif
