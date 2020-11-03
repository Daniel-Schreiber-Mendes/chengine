#ifndef WINDOW_MOD_H
#define WINDOW_MOD_H
#include <chengine-dev/engine.h>
#include <GLFW/glfw3.h>
#include <GL/glew.h>


extern GLFWwindow* window;

void window_init(void);
void window_terminate(void);
void window_fullscreen_set(bool set);
void window_fullscreen_switch(void);
void window_vsync_set(bool set);
void window_title_set(char const *title);
void window_size_set(uint16_t new_width, uint16_t new_height);
void window_cursor_visible_set(bool set);
void window_poll_events(void);


typedef struct
{
	int const width, height;
}
FramebufferSizeCommand;


typedef struct
{
	int const key, scancode, action, mods;
}
KeyCommand;


typedef struct
{
	int const button, action, mods;
}
MouseButtonCommand;


typedef struct
{
	int const jid, event;
}
JoystickCommand;


typedef struct
{
	float yoffset;
}
ScrollCommand;


typedef struct
{
	int const key, scancode, action, mods;
}
KeyEvent;


typedef struct
{
	int const button, action, mods;
}
MouseButtonEvent;


typedef struct
{
	int const jid, event;
}
JoystickEvent;


#endif