#ifndef WINDOW_H
#define WINDOW_H
#include "../render/render.h"
#include "../input/input.h"
#include <stdbool.h>
#include <assert.h>
#include <GLFW/glfw3.h>

extern GLFWwindow* window;

void window_init(char const* const title, uint16_t const n_width, uint16_t const n_height);
void window_terminate(void);
void window_fullscreen_set(bool const set);
void window_fullscreen_switch(void);
void window_vsync_set(bool const set);

void inputManager_init(void);
void inputManager_inputReceiver_add(EntityId const entity);


#endif