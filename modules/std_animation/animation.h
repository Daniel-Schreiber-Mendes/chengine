#ifndef ANIMATION_H
#define ANIMATION_H
#include "../std_render/render.h"
#include <boost/preprocessor/seq/for_each_i.hpp>


typedef struct
{
	uint8_t frameCount, frameDelay; //frameCount is the number of keyframes, framedelay is the number of frames one keyframe is shown 
	Texture **frames;
}
Animation;


typedef struct
{
	uint8_t frame;
	Animation *animation;
}
Animatable;


void animation_task(void);

#define animation_frame_set(r, a, i, texture)\
	(a)->frames[i] = texture;

#define animation_construct(a, framedelay, framecount, ...)\
	che_assert(framecount);\
	(a)->frameCount = framecount;\
	(a)->frameDelay = framedelay;\
	(a)->frames = che_malloc(sizeof(Texture*) * framecount);\
	BOOST_PP_SEQ_FOR_EACH_I(animation_frame_set, a, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))


void animation_destruct(Animation *a);


#endif