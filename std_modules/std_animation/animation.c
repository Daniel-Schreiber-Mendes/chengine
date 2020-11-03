#include "animation.h"


void animation_task(void)
{
	checs_component_mut_use(Animatable, a);
	checs_component_mut_use(Renderable, r);

	checs_components_foreach(Animatable, a, entity)
	{
		checs_component_get(Renderable, r, entity);
		a->frame = (a->frame + 1) % (a->animation->frameCount * a->animation->frameDelay);

		r->texture = a->animation->frames[(uint8_t)(a->frame / a->animation->frameDelay)];
	}
}


void animation_destruct(Animation *const a)
{
	che_free(a->frames);
}