#ifndef WINDOW_MOD_H
#define WINDOW_MOD_H
#include <checs/checs.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

void window_import(void);
void window_init(void);
void window_terminate(void);
void window_fullscreen_set(bool set);
void window_fullscreen_switch(void);
void window_vsync_set(bool set);
void window_title_set(char const *title);
void window_size_set(uint16_t new_width, uint16_t new_height);
void window_cursor_visible_set(bool set);
void window_poll_events(void);
void window_buffers_swap(void);
void window_cursorpos_get(double *xpos, double *ypos);
int window_key_get(int key);
void window_size_get(uint16_t *width, uint16_t *height);


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
	float yoffset;
}
ScrollCommand;


#endif