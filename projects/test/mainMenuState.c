#include "custom.h"


static Texture tileset;
static Texture cobble;


static void window_close_command_callback(checs_command_parameters);
static void framebuffer_size_command_callback(checs_command_parameters);
static void key_command_callback(checs_command_parameters);
static void mouse_button_command_callback(checs_command_parameters);
static void joystick_command_callback(checs_command_parameters);
static void scroll_command_callback(checs_command_parameters);
static void chunk_unload_callback(Chunk const *const c);
static EntityId chunk_load_callback(void);
static void draw_callback(void);



void mainMenuState_construct(MainMenuState *const s)
{
    {    
        s->b.destruct = &mainMenuState_destruct;
        s->b.update = &mainMenuState_update;
        s->b.draw = &mainMenuState_draw;
        s->b.drawing = true;
        s->b.updating = true;
    }

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

        program_construct(&r->program, "./resources/shader/vertex.glsl", "./resources/shader/fragment.glsl");
    }

    {
        EntityId player = checs_entity_generate(Renderable, Transform, Velocity, KeyInput, Collidable, SoundSource);
        checs_component_get_once(Renderable, r, player);
        checs_component_get_once(Transform, t, player);
        checs_component_get_once(SoundSource, ss, player);
        checs_component_get_once(Collidable, c, player);
        renderable_construct(r);
        soundSource_construct(ss, "./resources/error/errorMusic.wav");
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

        program_construct(&r->program, "./resources/shader/vertex.glsl", "./resources/shader/fragment.glsl");
    }
}


void mainMenuState_destruct(State *const state)
{

}


void mainMenuState_update(State *const state)
{
    checs_update();
}


void mainMenuState_draw(State *const state)
{
    checs_draw();
}


void stateMachine_first_state_push(void)
{
    stateMachine_state_push(MainMenuState, mainMenuState_construct);
}


void checs_init_config(void)
{
    checs_init
    (
        1,                   //systemUpdateCount
        1,                   //systemDrawCount,
        3,                   //taskUpdateCount
        0,                   //taskDrawCount
        1,                   //tag_count
        8,                   //componentCount
        19,                  //maxEntitysHint
        StandardCommandCount,//commandCount
        StandardEventCount   //eventCount
    );

    checs_command_subscribe(WindowCloseCommand, window_close_command_callback);
    checs_command_subscribe(FramebufferSizeCommand, framebuffer_size_command_callback);
    checs_command_subscribe(KeyCommand, key_command_callback);
    checs_command_subscribe(MouseButtonCommand, mouse_button_command_callback);
    checs_command_subscribe(JoystickCommand, joystick_command_callback);
    checs_command_subscribe(ScrollCommand, scroll_command_callback);
    checs_component_register(Renderable, 18, renderable_destruct);
    checs_component_register(Transform, 19, NULL);
    checs_component_register(KeyInput, 1, NULL);
    checs_component_register(SoundSource, 1, soundSource_destruct);
    checs_component_register(Camera, 1, NULL);
    checs_component_register(Chunk, 16, NULL);
    checs_component_register(Velocity, 3, NULL);
    checs_component_register(Collidable, 2, NULL);
    checs_event_register(KeyEventData, KeyEvent, 4);
    checs_event_register(MouseButtonEventData, MouseButtonEvent, 1);
    checs_event_register(JoystickEventData, JoystickEvent, 1);
    checs_system_register(render_system, CHECS_ON_DRAW, 1, 5, RENDER_SYSTEM_COMPONENTS);
    checs_system_register(chunk_loading_system, CHECS_ON_UPDATE, 1, 5, CHUNK_LOADING_SYSTEM_COMPONENTS);
    checs_task_register(physics_task, CHECS_ON_UPDATE);
    checs_task_register(input_task, CHECS_ON_UPDATE);
    checs_task_register(movement_task, CHECS_ON_UPDATE);
}


void che_window_config(void)
{
    window_fullscreen_set(false);
    //window_size_set(700, 300);
    window_title_set("Che Engine");
    window_vsync_set(true);
}


void che_init_config(void)
{
    render_system_init();
    physics_task_init();

    texture_construct_from_file(&cobble, "./resources/textures/circle.png");
    texture_construct_from_file(&tileset, "./resources/textures/tilemap.png");
    chunk_loading_system_init(chunk_load_callback, chunk_unload_callback, 2, 1, 32, &tileset, 32);
}


void che_terminate_config(void)
{
    physics_task_terminate();
    render_system_terminate();
    chunk_loading_system_terminate();
    texture_destruct(&cobble);
    texture_destruct(&tileset);
}


static void chunk_unload_callback(Chunk const *const c)
{

}


static EntityId chunk_load_callback(void)
{
    EntityId chunk = checs_entity_generate(Chunk, Renderable, Transform);
    chunk_construct(chunk);
    return chunk;
}


static void window_close_command_callback(checs_command_parameters)
{
    stateMachine_running_set(false);
}


static void framebuffer_size_command_callback(checs_command_parameters)
{
    #define sizeData ((FramebufferSizeData*)data)
    checs_component_get_once(Camera, c, checs_entity_get_by_tag(CameraTag));
    camera_default_resize(c, sizeData->width, sizeData->height);
}


static void key_command_callback(checs_command_parameters)
{
    #define keyData ((KeyData*)data)
    if (keyData->key == GLFW_KEY_F11 && keyData->action == GLFW_PRESS)
    {
        window_fullscreen_switch();
    }
}


static void mouse_button_command_callback(checs_command_parameters)
{
    #define buttonData ((MouseButtonData*)data)
}


static void joystick_command_callback(checs_command_parameters)
{
    #define cData ((JoystickData*)data)
}


static void scroll_command_callback(checs_command_parameters)
{
    #define offset ((ScrollData*)data)
    checs_component_get_once(Camera, c, checs_entity_get_by_tag(CameraTag));
    camera_default_zoom(c, offset->yoffset);
}


static void draw_callback(void)
{
    //render_system_imm_rectangle_draw((vec4){0.0f, 0.0f, 1.0f, 1.0f}, (vec3){0.0f, 0.0f, 0.1f}, (vec2){0.3f, 0.3f});
    //render_system_imm_rectangle_draw((vec4){1.0f, 0.0f, 0.0f, 1.0f}, (vec3){-1.0f, -1.0f, 1.0f}, (vec2){0.2f, 0.2f});
}