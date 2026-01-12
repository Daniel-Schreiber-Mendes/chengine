#include "animation.h"


void animation_import(void)
{
	checs_component_register(<Animation>, sizeof(Animation), NULL, NULL, 1 << <Transform> | 1 << <Renderable>);
	checs_tasks_register(CHECS_ON_UPDATE, animation_task, 1);
}


void animationPattern_construct(AnimationPattern *p, uint8_t frameCount, Texture **t)
{
	assert(frameCount);
	p->frames = malloc(frameCount * sizeof(Texture*));
	p->frameCount = frameCount;
	memcpy(p->frames, t, frameCount * sizeof(Texture*));
}


void animation_task(void)
{
    for (uintEC i=0, e=0; checs_components_iterate(<Animation>, &e, i); ++i)
    {
    	Animation *a = checs_component_get(<Animation>, e);
    	Renderable *r = checs_component_get(<Renderable>, e);
    	
    	assert(a->pattern);
    	assert(a->frameDelay);

    	++a->frame;
    	a->frame = a->frame % (a->pattern->frameCount * a->frameDelay);

		r->texture = a->pattern->frames[(uint8_t)(a->frame / a->frameDelay)];
    }
}


void animationPattern_destruct(AnimationPattern *p)
{
	free(p->frames);
}