#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <assert.h>
#include "../commands/commands.h"
#include "../render/render.h"


extern GLFWwindow* window;

void window_init(char const* const title, uint16_t const n_width, uint16_t const n_height);
void window_terminate(void);
void window_fullscreen_set(bool const set);
void window_fullscreen_switch(void);