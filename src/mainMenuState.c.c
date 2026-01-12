#include <custom.h>

Texture cobble, circle, player_tex, player1_tex, grass;
AnimationPattern player_animationPattern;
Image tileset_image;

static void key_command_callback(KeyCommand *c);
static void scroll_command_callback(ScrollCommand *com);
static void mouseButton_command_callback(MouseButtonCommand *com);
static void window_close_command_callback(void *c);
static void framebuffer_size_command_callback(FramebufferSizeCommand *com);


void mainMenuState_construct(State *const s)
{
    s->destruct = &mainMenuState_destruct;
    s->update = &mainMenuState_update;
    s->draw = &mainMenuState_draw;
    s->events = &mainMenuState_events;


    checs_command_subscribe(NULL_TEMPLATE, window_close_command_callback);
    checs_command_subscribe(NULL_TEMPLATE, framebuffer_size_command_callback);
    checs_command_subscribe(NULL_TEMPLATE, key_command_callback);
    checs_command_subscribe(NULL_TEMPLATE, scroll_command_callback);
    checs_command_subscribe(NULL_TEMPLATE, mouseButton_command_callback);

    window_fullscreen_set(false);

    tileset_image = textureManager_image_load("./resources/textures/tilemap.png");
    texture_construct(&cobble, 128, 32, 32, 32, &tileset_image);
    texture_construct(&grass, 0, 0, 32, 32, &tileset_image);
    texture_construct(&circle, 0, 128, 32, 32, &tileset_image);
    texture_construct(&player_tex, 0, 160, 28, 28, &tileset_image);
    texture_construct(&player1_tex, 0, 192, 28, 28, &tileset_image);
    render_rectBatch_image_set(&tileset_image);

    Texture* frames[] =
    {
        &player_tex,
        &player1_tex
    };
    animationPattern_construct(&player_animationPattern, 2, frames);

    checs_state_push(&game_state_construct);
}


void mainMenuState_destruct(State *const state)
{
    animationPattern_destruct(&player_animationPattern);
}


void mainMenuState_events(State *const state)
{

}


void mainMenuState_update(State *const state)
{

}


void mainMenuState_draw(State *const state)
{

}


static void key_command_callback(KeyCommand *kc)
{
    if (kc->key == GLFW_KEY_F11 && kc->action == GLFW_PRESS)
    {
        window_fullscreen_switch();
    }
    else if (kc->key == GLFW_KEY_C && kc->action == GLFW_PRESS)
    {
        for (int i=0; i<20; ++i)
        {
            EntityId e = checs_entity(1 << 5 | 1 << 2 | 1 << 7);

            checs_tags_add(1 << 1, e);


            Transform *t = checs_component_get(1, e);
            t->pos[0] = i * 0.003f;
            t->pos[1] = i * 0.003f;

            Renderable *r = checs_component_get(5, e);
            r->scale[0] = 0.25f;
            r->scale[1] = 0.25f;
            r->texture = &circle;
            r->type = RT_RECT;

            Collidable *c = checs_component_get(7, e);
            c->r = 0.125;
            c->mass = 0.5;
            c->behaviourType = DYNAMIC;
            c->type = CIRCLE;
        } 
    }
    else if (kc->key == GLFW_KEY_L && kc->action == GLFW_PRESS)
    {
        EntityId e = checs_entity_get_by_tag(1 << 1);
        
        if (e)
            checs_entity_erase(e);
    }
}

static void scroll_command_callback(ScrollCommand *com)
{
    Camera *c;
    EntityId entity;
    for (uintEC i=0; c = checs_components_foreach(6, &entity, i); ++i)
    {
        c->zoom += c->zoom * com->yoffset * 0.1f;
    }
}


static void mouseButton_command_callback(MouseButtonCommand *com)
{
    if (com->action == GLFW_PRESS)
    {
        EntityId player = checs_entity_get_by_name("player");
        Transform *t = checs_component_get(1, player);
        Velocity *v = checs_component_get(2, player);

        t->pos[0] = 0;
        t->pos[1] = 0;
        v->vel[0] = 0;
        v->vel[1] = 0;
    }
}


static void window_close_command_callback(void *c)
{
    checs_running_set(false);
}


static void framebuffer_size_command_callback(FramebufferSizeCommand *com)
{
    Camera *c;
    EntityId entity;
    for (uintEC i=0; c = checs_components_foreach(6, &entity, i); ++i)
    {
        camera_resize(c, com->width, com->height);
        //camera_resize(c, 500, 500);
    }
}
