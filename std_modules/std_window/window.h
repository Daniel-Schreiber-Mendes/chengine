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


#define WindowCloseCommand     0
#define FramebufferSizeCommand 1
#define KeyCommand 			   2
#define MouseButtonCommand     3
#define JoystickCommand        4
#define ScrollCommand 		   5
#define StandardCommandCount   6


typedef struct
{
	int const width, height;
}
FramebufferSizeData;


typedef struct
{
	int const key, scancode, action, mods;
}
KeyData;


typedef struct
{
	int const button, action, mods;
}
MouseButtonData;


typedef struct
{
	int const jid, event;
}
JoystickData;


typedef struct
{
	float yoffset;
}
ScrollData;


#define KeyEvent 		   0
#define MouseButtonEvent   1
#define JoystickEvent      2
#define StandardEventCount 3


typedef struct
{
	int const key, scancode, action, mods;
}
KeyEventData;


typedef struct
{
	int const button, action, mods;
}
MouseButtonEventData;


typedef struct
{
	int const jid, event;
}
JoystickEventData;


#endif