#include "MainMenuState.h"


void mainMenuState_construct(MainMenuState *const s)
{
    s->b.destruct = &mainMenuState_destruct;
    s->b.update = &mainMenuState_update;
    s->b.draw = &mainMenuState_draw;

    s->b.drawing = true;
    s->b.updating = true;

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

    vertexArray_construct(&s->vao);
    vertexBuffer_construct(&s->vbo, sizeof(GLfloat) * 5 * 4, s->positions);
    indexBuffer_construct(&s->ibo, sizeof(GLuint) * 6, 6, s->indices);
    texture_construct(&s->tex, "../resources/textures/cobble.png");
    program_create(&s->program, "../resources/shader/vertex.glsl", "../resources/shader/fragment.glsl");
    vertexBufferLayout_construct(&s->vbl, 2);

    vertexBufferLayout_element_add(&s->vbl, (VertexBufferLayoutElement){3, GL_FLOAT, GL_FALSE});
    vertexBufferLayout_element_add(&s->vbl, (VertexBufferLayoutElement){2, GL_FLOAT, GL_FALSE});
    vertexArray_buffer_add(s->vao, s->vbo, s->vbl);
    glClearColor(0.5, 0.5, 0.5, 1);
    glm_ortho(-4, 4, -1.5, 1.5, -1, 1, s->proj);
    //glm_translate_x(proj, 1);

    program_uniformMat4f_set(s->program, "u_mvp", s->proj);
    program_uniform1i_set(s->program, "u_texture", 0);
}


void mainMenuState_destruct(State *const state)
{
    MainMenuState *const s = (MainMenuState*)state;

    vertexBuffer_destruct(&s->vbo);
    indexBuffer_destruct(&s->ibo);
    vertexArray_destruct(&s->vao);
    vertexBufferLayout_destruct(&s->vbl);
    program_destroy(s->program);
    texture_destruct(&s->tex);
}


void mainMenuState_update(State *const state)
{
    MainMenuState *const s = (MainMenuState*)state;
}


void mainMenuState_draw(State *const state)
{
    MainMenuState *const s = (MainMenuState*)state;

    glClear(GL_COLOR_BUFFER_BIT);
    {
        renderer_draw(s->vao, &s->ibo, s->program);
    }
    glfwSwapBuffers(window);
    glfwPollEvents();
}