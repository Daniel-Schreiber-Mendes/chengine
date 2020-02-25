#include "custom.h"


void mainMenuState_construct(MainMenuState *const s)
{
    window_init("Che Engine", 640, 480);
    inputManager_init();
    window_vsync_set(true);

    uint8_t a;
    uint32_t b;
    wavFile_load("../resources/error/errorSound.wav", &a, &b);

    render_system_init();

    {    
        s->b.destruct = &mainMenuState_destruct;
        s->b.update = &mainMenuState_update;
        s->b.draw = &mainMenuState_draw;
        s->b.drawing = true;
        s->b.updating = true;
    }


    EntityId camera = checs_entity_generate(Transform, KeyInput);
    checs_component_get_once(Transform, transform, camera);
    checs_entity_tag_add(camera, CameraTag);
    memset(transform, 0, sizeof(Transform));


    EntityId player = checs_entity_generate(Renderable, Transform, KeyInput);
    checs_component_get_once(Renderable, r, player);


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

    program_uniform1i_set(r->program, "u_texture", 0);
}


void mainMenuState_destruct(State *const state)
{
    MainMenuState *const s = (MainMenuState*)state;

    checs_component_get_once(Renderable, r, 0);

    elementBuffer_destruct(&r->ebo);
    vertexArray_destruct(&r->vao);
    program_destroy(r->program);

    vertexBuffer_destruct(&s->vbo);
    vertexBufferLayout_destruct(&s->vbl);
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