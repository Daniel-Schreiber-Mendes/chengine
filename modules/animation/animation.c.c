#include "animation.h"


void animation_import(void)
{
	checs_component_register(<Animatable>, Animatable, 1, NULL, NULL);
	checs_tasks_register(CHECS_ON_UPDATE, animation_task);
}


void animation_task(void)
{
	Animatable *a;
	Renderable *r;

	checs_components_foreach(<Animatable>, a, entity, ({
		r = checs_component_get(<Renderable>, entity);
		a->frame = (a->frame + 1) % (a->animation->frameCount * a->animation->frameDelay);

		r->texture = a->animation->frames[(uint8_t)(a->frame / a->animation->frameDelay)];
	}))
}


void animation_destruct(Animation *const a)
{
	che_free(a->frames);
}
