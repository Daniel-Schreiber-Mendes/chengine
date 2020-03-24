#include "custom.h"


void draw_callback(void);

void draw_callback(void)
{
    //render_system_imm_rectangle_draw((vec4){0.0f, 0.0f, 1.0f, 1.0f}, (vec3){0.0f, 0.0f, 0.1f}, (vec2){0.3f, 0.3f});
    //render_system_imm_rectangle_draw((vec4){1.0f, 0.0f, 0.0f, 1.0f}, (vec3){-1.0f, -1.0f, 1.0f}, (vec2){0.2f, 0.2f});
}


void mainMenuState_construct(MainMenuState *const s)
{
    {    
        s->b.destruct = &mainMenuState_destruct;
        s->b.update = &mainMenuState_update;
        s->b.draw = &mainMenuState_draw;
        s->b.drawing = true;
        s->b.updating = true;
    }

    window_vsync_set(true);
    window_fullscreen_set(true);
    window_cursor_visible_set(true);

    {
        EntityId camera = checs_entity_generate(Transform, Velocity, Camera);
        checs_entity_tag_add(camera, CameraTag);;
        checs_component_get_once(Camera, c, camera);
        camera_construct(c);
        c->zoom = 4;
    }



    render_system_custom_draw_callback_set(draw_callback);

    {
        EntityId enemy = checs_entity_generate(Renderable, Transform, Collidable);
        checs_component_get_once(Renderable, r, enemy);
        checs_component_get_once(Collidable, c, enemy);
        renderable_construct(r);
        //c->r = 2.5f;
        c->bb[0] = 5;
        c->bb[1] = 5;
        c->behaviourType = STATIC;
        c->type = RECTANGLE;

        GLfloat positions[5 * 4] = 
        {//  3 x position  2 x tex
            0.0,  0.0, 1, 0, 0,
            5.0,  0.0, 1, 1, 0,
            5.0, -5.0, 1, 1, 1,
            0.0, -5.0, 1, 0, 1
        };

        GLuint elements[6] = 
        {
            0, 1, 2,
            2, 3, 0
        };


        VertexBuffer vbo;
        VertexBufferLayout vbl;
        ElementBuffer ebo;
        elementBuffer_construct(&ebo, elements);
        vertexBuffer_construct(&vbo, positions);
        vertexBufferLayout_construct(&vbl, 2);
        vertexBufferLayout_element_add(&vbl, (VertexBufferLayoutElement){3, GL_FLOAT, GL_FALSE}); //pos coords
        vertexBufferLayout_element_add(&vbl, (VertexBufferLayoutElement){2, GL_FLOAT, GL_FALSE}); //tex coords
        vertexArray_buffer_add(&r->vao, vbo, vbl);
        r->mode = GL_TRIANGLES;
        r->renderableType = ELEMENTS;
        r->elementCount = 6;
        r->texture = &cobble;

        program_construct(&r->program, "../resources/shader/vertex.glsl", "../resources/shader/fragment.glsl");
    }

    {
        EntityId player = checs_entity_generate(Renderable, Transform, Velocity, KeyInput, Collidable, SoundSource);
        checs_component_get_once(Renderable, r, player);
        checs_component_get_once(Transform, t, player);
        checs_component_get_once(SoundSource, ss, player);
        checs_component_get_once(Collidable, c, player);
        renderable_construct(r);
        soundSource_construct(ss, "../resources/error/errorMusic.wav");
        //c->r = 2.5f;
        c->bb[0] = 5;
        c->bb[1] = 5;
        c->behaviourType = DYNAMIC;
        c->type = RECTANGLE;
        t->pos[0] = 5;

        {
            checs_component_get_once(Camera, cam, checs_entity_get_by_tag(CameraTag));
            camera_target_set(cam, t);
        }


        GLfloat positions[5 * 4] = 
        {//  3 x position  2 x tex
            0.0,  0.0, 1, 0, 0,
            5.0,  0.0, 1, 1, 0,
            5.0, -5.0, 1, 1, 1,
            0.0, -5.0, 1, 0, 1
        };

        GLuint elements[6] = 
        {
            0, 1, 2,
            2, 3, 0
        };


        VertexBuffer vbo;
        VertexBufferLayout vbl;
        ElementBuffer ebo;
        elementBuffer_construct(&ebo, elements);
        vertexBuffer_construct(&vbo, positions);
        vertexBufferLayout_construct(&vbl, 2);
        vertexBufferLayout_element_add(&vbl, (VertexBufferLayoutElement){3, GL_FLOAT, GL_FALSE}); //pos coords
        vertexBufferLayout_element_add(&vbl, (VertexBufferLayoutElement){2, GL_FLOAT, GL_FALSE}); //tex coords
        vertexArray_buffer_add(&r->vao, vbo, vbl);
        r->mode = GL_TRIANGLES;
        r->renderableType = ELEMENTS;
        r->elementCount = 6;
        r->texture = &cobble;

        program_construct(&r->program, "../resources/shader/vertex.glsl", "../resources/shader/fragment.glsl");
    }
}


void mainMenuState_destruct(State *const state)
{

}


void mainMenuState_update(State *const state)
{
    checs_tasks_call(CHECS_ON_UPDATE);
    checs_systems_call(CHECS_ON_UPDATE);
}


void mainMenuState_draw(State *const state)
{
    checs_tasks_call(CHECS_ON_DRAW);
    checs_systems_call(CHECS_ON_DRAW);
}