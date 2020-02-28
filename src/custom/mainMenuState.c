#include "custom.h"


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

    {
        EntityId camera = checs_entity_generate(Transform, KeyInput, Camera);
        checs_component_get_once(Transform, t, camera);
        checs_component_get_once(Camera, c, camera);
        checs_entity_tag_add(camera, CameraTag);
        transform_construct(t);
        camera_construct(c);
    }


    GLuint program;
    texture_construct(&s->tex, "../resources/textures/ui.png");
    program_create(&program, "../resources/shader/vertex.glsl", "../resources/shader/fragment.glsl");
    program_uniform1i_set(program, "u_texture", 0);

    
    {
        EntityId player = checs_entity_generate(Renderable, Transform);
        checs_component_get_once(Renderable, r, player);
        checs_component_get_once(Transform, t, player);
        transform_construct(t);


        GLfloat positions[5 * 4] = 
        {
        //   3 x position  2 x tex
            -0.3, -0.3, 0, 0, 0,
             0.3, -0.3, 0, 1, 0,
             0.3,  0.3, 0, 1, 1,
            -0.3,  0.3, 0, 0, 1
        };

        GLuint elements[6] = 
        {
            0, 1, 2,
            2, 3, 0
        };


        GLuint vbo;
        VertexBufferLayout vbl;
        vertexArray_construct(&r->vao);
        elementBuffer_construct(&r->ebo, elements);
        vertexBuffer_construct(&vbo, positions);
        vertexBufferLayout_construct(&vbl, 2);
        vertexBufferLayout_element_add(&vbl, (VertexBufferLayoutElement){3, GL_FLOAT, GL_FALSE}); //pos coords
        vertexBufferLayout_element_add(&vbl, (VertexBufferLayoutElement){2, GL_FLOAT, GL_FALSE}); //tex coords
        vertexArray_buffer_add(r->vao, vbo, vbl);
        r->program = program;
    }

    
    {
        EntityId player = checs_entity_generate(Renderable, Transform);
        checs_component_get_once(Renderable, r, player);
        checs_component_get_once(Transform, t, player);
        transform_construct(t);


        GLfloat positions[5 * 4] = 
        {
        //   3 x position  2 x tex
            0, 0, 0, 0.0, 0.0,
            1, 0, 0, 0.5, 0.0,
            1, 1, 0, 0.5, 0.5,
            0, 1, 0, 0.0, 0.5
        };

        GLuint elements[6] = 
        {
            0, 1, 2,
            2, 3, 0
        };


        VertexBuffer vbo;
        VertexBufferLayout vbl;
        vertexArray_construct(&r->vao);
        elementBuffer_construct(&r->ebo, elements);
        vertexBuffer_construct(&vbo, positions);
        vertexBufferLayout_construct(&vbl, 2);
        vertexBufferLayout_element_add(&vbl, (VertexBufferLayoutElement){3, GL_FLOAT, GL_FALSE}); //pos coords
        vertexBufferLayout_element_add(&vbl, (VertexBufferLayoutElement){2, GL_FLOAT, GL_FALSE}); //tex coords
        vertexArray_buffer_add(r->vao, vbo, vbl);
        r->program = program;
    }
}


void mainMenuState_destruct(State *const state)
{
    MainMenuState *const s = (MainMenuState*)state;

    checs_component_get_once(Renderable, r, 0);

    elementBuffer_destruct(&r->ebo);
    vertexArray_destruct(&r->vao);
    program_destroy(r->program);

    texture_destruct(&s->tex);
}


void mainMenuState_update(State *const state)
{
    checs_systems_call(ON_UPDATE);
    checs_tasks_call(ON_UPDATE);
}


void mainMenuState_draw(State *const state)
{
    checs_systems_call(ON_DRAW);
    checs_tasks_call(ON_DRAW);
}