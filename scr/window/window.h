#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <assert.h>


extern GLFWwindow* window;

void window_init(char const* const title);
void window_terminate(void);
