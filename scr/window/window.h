#pragma once
#include "../utility/utility.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <stdio.h>


extern GLFWwindow* window;
extern GLFWmonitor* monitor;


void 		 window_init(char const* const title);
void 		 window_terminate(void);
