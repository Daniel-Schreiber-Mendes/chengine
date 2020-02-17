#include "MainMenuState.h"


void mainMenuState_construct(MainMenuState *const s)
{
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
        -1, -1, 0, 0, 0,
         1, -1, 0, 1, 0,
         1,  1, 0, 1, 1,
        -1,  1, 0, 0, 1
    };

    GLuint indices[6] = 
    {
        0, 1, 2,
        2, 3, 0
    };

    memcpy(s->positions, positions, sizeof(positions));
    memcpy(s->indices, indices, sizeof(indices));

    vertexArray_construct(&r->vao);
    indexBuffer_construct(&r->ibo, sizeof(GLuint) * 6, 6, s->indices);
    program_create(&r->program, "../resources/shader/vertex.glsl", "../resources/shader/fragment.glsl");

    vertexBuffer_construct(&s->vbo, sizeof(GLfloat) * 5 * 4, s->positions);
    texture_construct(&s->tex, "../resources/textures/cobble.png");

    vertexBufferLayout_construct(&s->vbl, 2);
    vertexBufferLayout_element_add(&s->vbl, (VertexBufferLayoutElement){3, GL_FLOAT, GL_FALSE});
    vertexBufferLayout_element_add(&s->vbl, (VertexBufferLayoutElement){2, GL_FLOAT, GL_FALSE});

    vertexArray_buffer_add(r->vao, s->vbo, s->vbl);


    glClearColor(0.5, 0.5, 0.5, 1);
    glm_ortho(-4, 4, -1.5, 1.5, -1, 1, s->proj);
    //glm_translate_x(proj, 1);

    program_uniformMat4f_set(r->program, "u_mvp", s->proj);
    program_uniform1i_set(r->program, "u_texture", 0);
}


void mainMenuState_destruct(State *const state)
{
    MainMenuState *const s = (MainMenuState*)state;

    EntityId e = 0;
    checs_component_use(Renderable, r);
    checs_component_get(Renderable, r, e);

    indexBuffer_destruct(&r->ibo);
    vertexArray_destruct(&r->vao);
    program_destroy(r->program);

    vertexBuffer_destruct(&s->vbo);
    vertexBufferLayout_destruct(&s->vbl);
    texture_destruct(&s->tex);
}


void mainMenuState_update(State *const state)
{
    systemManager_systems_call(ON_UPDATE);
    systemManager_tasks_call(ON_UPDATE);
}


void mainMenuState_draw(State *const state)
{
    systemManager_systems_call(ON_DRAW);
    systemManager_tasks_call(ON_DRAW);
}