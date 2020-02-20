#include "MainMenuState.h"


Camera c;

void mainMenuState_construct(MainMenuState *const s)
{
    window_init("Che Engine", 640, 480);

    {    
        s->b.destruct = &mainMenuState_destruct;
        s->b.update = &mainMenuState_update;
        s->b.draw = &mainMenuState_draw;
        s->b.drawing = true;
        s->b.updating = true;
    }

    EntityId e = checs_entity_generate(Renderable);
    checs_component_use(Renderable, r);
    checs_component_get(Renderable, r, e);

    GLfloat positions[5 * 4] = 
    {
        -0.3, -0.3, 0, 0, 0,
         0.3, -0.3, 0, 0.3, 0,
         0.3,  0.3, 0, 0.3, 0.3,
        -0.3,  0.3, 0, 0, 0.3
    };

    GLuint elements[6] = 
    {
        0, 1, 2,
        2, 3, 0
    };

    vertexArray_construct(&r->vao);
    elementBuffer_construct(&r->ebo, sizeof(GLuint) * 6, 6, elements);
    program_create(&r->program, "../resources/shader/vertex.glsl", "../resources/shader/fragment.glsl");

    vertexBuffer_construct(&s->vbo, sizeof(GLfloat) * 5 * 4, positions);
    texture_construct(&s->tex, "../resources/textures/cobble.png");

    vertexBufferLayout_construct(&s->vbl, 2);
    vertexBufferLayout_element_add(&s->vbl, (VertexBufferLayoutElement){3, GL_FLOAT, GL_FALSE});
    vertexBufferLayout_element_add(&s->vbl, (VertexBufferLayoutElement){2, GL_FLOAT, GL_FALSE});

    vertexArray_buffer_add(r->vao, s->vbo, s->vbl);

    glClearColor(0.3, 0.4, 1, 0);
    camera_construct(&c, -1.6, 1.6, -0.9, 0.9);

    program_uniformMat4f_set(r->program, "u_vpm", c.vpm);
    program_uniform1i_set(r->program, "u_texture", 0);
}


void mainMenuState_destruct(State *const state)
{
    MainMenuState *const s = (MainMenuState*)state;

    EntityId e = 0;
    checs_component_use(Renderable, r);
    checs_component_get(Renderable, r, e);

    elementBuffer_destruct(&r->ebo);
    vertexArray_destruct(&r->vao);
    program_destroy(r->program);

    checs_entity_erase(e);

    vertexBuffer_destruct(&s->vbo);
    vertexBufferLayout_destruct(&s->vbl);
    texture_destruct(&s->tex);
}


void mainMenuState_update(State *const state)
{
    checs_component_use(Renderable, r);
    checs_component_get(Renderable, r, 0);
    program_uniformMat4f_set(r->program, "u_vpm", c.vpm);
    systemManager_systems_call(ON_UPDATE);
    systemManager_tasks_call(ON_UPDATE);
}


void mainMenuState_draw(State *const state)
{
    systemManager_systems_call(ON_DRAW);
    systemManager_tasks_call(ON_DRAW);
}