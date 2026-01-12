#include "window.h"


static uint16_t std_width = 400, std_height = 400; //if the window mode is switched back to not fullscreen, set this as its size
static GLFWmonitor *monitor;
static bool fullscreen;
static GLFWwindow* window;


static void window_close_callback(GLFWwindow* _window);
static void framebuffer_size_callback(GLFWwindow *const _window, int const _width, int const _height);
static void key_callback(GLFWwindow* _window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow* _window, int button, int action, int mods);
static void joystick_callback(int jid, int event);
static void scroll_callback(GLFWwindow* _window, double const xoffset, double const yoffset);
static void cursor_pos_callback(GLFWwindow *_window, double xpos, double ypos);


void window_import()
{
    checs_command_register(NULL_TEMPLATE);
    checs_command_register(NULL_TEMPLATE);
    checs_command_register(NULL_TEMPLATE);
    checs_command_register(NULL_TEMPLATE);
    checs_command_register(NULL_TEMPLATE);
}



void window_init(void)
{
    assert(glfwInit());
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    monitor = glfwGetPrimaryMonitor();
    assert(monitor);
    window = glfwCreateWindow(std_width, std_height, "", NULL, NULL);
    assert(window);
    glfwMakeContextCurrent(window);
    assert(glewInit() == GLEW_OK);

    glfwSetWindowCloseCallback(window, window_close_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetWindowMonitor(window, NULL, 200, 200, std_width, std_height, 0);   
    framebuffer_size_callback(window, std_width, std_height);
    //we have to call this manually once after the window was created because it does not call this on creation
}


void window_terminate(void)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}


void window_fullscreen_set(bool const set)
{
    if (set && !fullscreen)
    {
        int videoModeCount;
        GLFWvidmode const *const mode = &glfwGetVideoModes(monitor, &videoModeCount)[videoModeCount - 1];
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else if (!set && fullscreen)
    {
        glfwSetWindowMonitor(window, NULL, 200, 200, std_width, std_height, 0);    
    }
    fullscreen = set;
}


void window_fullscreen_switch(void)
{
    window_fullscreen_set(!fullscreen);
}


void window_vsync_set(bool const set)
{
    glfwSwapInterval(set);
}


void window_title_set(char const *title)
{
    glfwSetWindowTitle(window, title);
}


void window_size_set(uint16_t const new_width, uint16_t const new_height)
{
    if (!fullscreen)
    {
        glfwSetWindowSize(window, std_width = new_width, std_height = new_height);
    }
}

/*
#define GLFW_CURSOR_NORMAL          0x00034001
#define GLFW_CURSOR_HIDDEN          0x00034002
because HIDDEN is just one more than NORMAL, we can use the input diredctly:
if set is true(1), add 0 to NORMAL, if it is false(0) add 1.
this means add the negation of set to NORMAL
*/
void window_cursor_visible_set(bool const set)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + !set);
}


void window_poll_events(void)
{
    glfwPollEvents();
}


void window_buffers_swap(void)
{
    glfwSwapBuffers(window);
}


void window_cursorpos_get(double *xpos, double *ypos)
{
    glfwGetCursorPos(window, xpos, ypos);
}


int window_key_get(int key)
{
    return glfwGetKey(window, key);
}


void window_size_get(uint16_t *width, uint16_t *height)
{
    *width = std_width;
    *height = std_height;
}


static void window_close_callback(GLFWwindow* _window)
{
    checs_command_publish(NULL_TEMPLATE, NULL);
}


static void framebuffer_size_callback(GLFWwindow *const _window, int const _width, int const _height)
{
    checs_command_publish(NULL_TEMPLATE, (&(FramebufferSizeCommand){_width, _height}));
}


static void key_callback(GLFWwindow* _window, int key, int scancode, int action, int mods)
{
    checs_command_publish(NULL_TEMPLATE, (&(KeyCommand){key, scancode, action, mods}));
}


static void mouse_button_callback(GLFWwindow* _window, int button, int action, int mods)
{
    checs_command_publish(NULL_TEMPLATE, (&(MouseButtonCommand){button, action, mods}));
}


static void scroll_callback(GLFWwindow* _window, double const xoffset, double const yoffset)
{
    /* we dont care about xoffset scrolling, so we are leaving that out */
    checs_command_publish(NULL_TEMPLATE, (&(ScrollCommand){yoffset}));
}
