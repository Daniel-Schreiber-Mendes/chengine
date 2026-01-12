#include <custom.h>


void game_state_construct(State *s)
{
    s->destruct = &game_state_destruct;
    s->update = &game_state_update;
    s->draw = &game_state_draw;
    s->events = &game_state_events;

    tilemap_generate();

    {
        EntityId player = checs_entity(1 << <Renderable> | 1<< <Transform> | 1 << <Velocity> | 1 << <Collidable> | 1 << <Animation>);

        checs_entity_name_add(player, "player");
        player = checs_entity_get_by_name("player");

        Transform *t = checs_component_get(<Transform>, player);
        t->pos[0] = 0.2;
        t->pos[1] = 0.2;

        Renderable *r = checs_component_get(<Renderable>, player);
        r->scale[0] = 0.2;
        r->scale[1] = 0.2;
        r->type = RT_RECT;
        r->texture = &player_tex;

        Collidable *c = checs_component_get(<Collidable>, player);
        c->r = 0.1f;
        c->mass = 10.f;
        c->behaviourType = DYNAMIC;
        c->type = CIRCLE;

        Animation *a = checs_component_get(<Animation>, player);
        a->pattern = &player_animationPattern;
        a->frameDelay = 20;


        EntityId camera = checs_entity(1 << <Transform> | 1 << <Velocity> | 1 << <Camera>);
        render_rectBatch_camera_set(camera);
        
        Camera *camera_c = checs_component_get(<Camera>, camera);
        camera_c->target = player;
        camera_c->zoom = 2;
        uint16_t width, height;
        window_size_get(&width, &height);
        camera_resize(camera_c, width, height);
    }

    {
        EntityId floor = checs_entity(1 << <Transform> | 1 << <Renderable> | 1 << <Collidable>);
        Transform *t = checs_component_get(<Transform>, floor);
        t->pos[0] = 0.f;
        t->pos[1] = -2.f;

        Renderable *r = checs_component_get(<Renderable>, floor);
        r->scale[0] = 4.f;
        r->scale[1] = 0.5f;
        r->texture = &cobble;
        r->type = RT_RECT;

        Collidable *c = checs_component_get(<Collidable>, floor);
        c->bb[0] = 2.f;
        c->bb[1] = 0.25f;
        c->mass = 0.5f;
        c->behaviourType = STATIC;
        c->type = AABB;
    }

    EntityId wall_left;
    {
        wall_left = checs_entity(1 << <Transform> | 1 << <Renderable> | 1 << <Collidable>);
        Transform *t = checs_component_get(<Transform>, wall_left);
        t->pos[0] = 2.f;
        t->pos[1] = 0.f;

        Renderable *r = checs_component_get(<Renderable>, wall_left);
        r->scale[0] = 0.5f;
        r->scale[1] = 4.f;
        r->texture = &cobble;
        r->type = RT_RECT;

        Collidable *c = checs_component_get(<Collidable>, wall_left);
        c->bb[0] = 0.25f;
        c->bb[1] = 2;
        c->mass = 0.5f;
        c->behaviourType = STATIC;
        c->type = AABB;
    }

    EntityId wall_right = checs_entity_copy(wall_left);
    Transform *t = checs_component_get(<Transform>, wall_right);
    t->pos[0] = -2;

    //physics_gravity_set((vec2){0, -0.002});

    //checs_entity_erase(wall_right);
}


void game_state_destruct(State *s)
{

}


void game_state_events(State *s)
{
	window_poll_events();
}


void game_state_update(State *s)
{
	checs_update();

	//printf("%f ms / frame, Entitys: %u, ms / entity %f\n", checs_ms_per_frame_get(), checs_component_count(<Transform>), checs_ms_per_frame_get() / checs_component_count(<Transform>));

	EntityId player = checs_entity_get_by_name("player");
    Velocity *const v = checs_component_get(<Velocity>, player);
    if (window_key_get(GLFW_KEY_D) == GLFW_PRESS)
    {
        v->vel[0] += 0.002f;
        if (v->vel[0] > 0.07)
            v->vel[0] = 0.07;
    }
    if (window_key_get(GLFW_KEY_A) == GLFW_PRESS)
    {
        v->vel[0] -= 0.002f;
        if (v->vel[0] < -0.07)
            v->vel[0] = -0.07;
    }
    if (window_key_get(GLFW_KEY_W) == GLFW_PRESS)
    {
        v->vel[1] += 0.002f;
        if (v->vel[1] > 0.07)
            v->vel[1] = 0.07;
    }
    if (window_key_get(GLFW_KEY_S) == GLFW_PRESS)
    {
        v->vel[1] -= 0.002f;
        if (v->vel[1] < -0.07)
            v->vel[1] = -0.07;
    }
    if (window_key_get(GLFW_KEY_R) == GLFW_PRESS)
    {
    	checs_reset();
    	checs_state_pop(s);
    	checs_state_push(&game_state_construct);
    	return;
    }
    if (window_key_get(GLFW_KEY_P) == GLFW_PRESS)
    {
        checs_task_active_set(movement_task, false);
    }
    Transform *t = checs_component_get(<Transform>, player);
    physics_partitions_center(t->pos);
}


void game_state_draw(State *s)
{
	checs_draw();
}