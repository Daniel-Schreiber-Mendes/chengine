#include <custom.h>


int main(void)
{
    checs_init();

    
    movement_import();
    animation_import();
    render_import();
    camera_import();
    window_import();
    physics_import();
    physics_gravity_import();
    tilemap_import();

    window_init();
    textureManager_init();
    render_task_init();
    text_init();

    window_fullscreen_set(true);
    window_title_set("Che Engine");
    window_vsync_set(true);

    checs_state_push(&mainMenuState_construct);


    checs_run();


    
    render_task_terminate();
    textureManager_terminate();
    window_terminate();
    text_terminate();
    

    checs_terminate();
    return 0;
}
