#include "../../engine.h"
#include <GL/glew.h>


static uint16_t width, height;
static GLFWmonitor *monitor;
static bool fullscreen;
GLFWwindow* window;


static void window_close_callback(GLFWwindow* _window);
static void framebuffer_size_callback(GLFWwindow *const _window, int const _width, int const _height);
static void key_callback(GLFWwindow* _window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow* _window, int button, int action, int mods);
static void joystick_callback(int jid, int event);
static void scroll_callback(GLFWwindow* _window, double const xoffset, double const yoffset);


void window_init(char const* const title, uint16_t const n_width, uint16_t const n_height)
{
    che_assert(glfwInit());
    monitor = glfwGetPrimaryMonitor();
    che_assert(window = glfwCreateWindow(width = n_width, height = n_height, title, NULL, NULL));
    glfwMakeContextCurrent(window);
    che_assert(glewInit() == GLEW_OK);
    glClearColor(1, 0, 1, 1); //set clear color to magenta so it is easyer to see if e.g the background got not rendered correctly


    glfwSetWindowCloseCallback(window, window_close_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetJoystickCallback(joystick_callback);
    glfwSetScrollCallback(window, scroll_callback);

    framebuffer_size_callback(window, width, height + 1);
    //we have to call this manually once after the window was created because it does not call this on creation
}


void window_terminate(void)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}


void window_fullscreen_set(bool const set)
{
    if ((fullscreen = set))
    {
        int videoModeCount;
        GLFWvidmode const *const mode = &glfwGetVideoModes(monitor, &videoModeCount)[videoModeCount - 1];
        glfwSetWindowMonitor(window, monitor, 0, 0, (width = mode->width), (height = mode->height), mode->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(window, NULL, 400, 200, 640, 480, 0);    
    }
}


void window_fullscreen_switch(void)
{
    window_fullscreen_set(!fullscreen);
}


void window_vsync_set(bool const set)
{
    glfwSwapInterval(set);
}


void window_cursor_visible_set(bool const set)
{
    /*
    #define GLFW_CURSOR_NORMAL          0x00034001
    #define GLFW_CURSOR_HIDDEN          0x00034002
    because HIDDEN is just one more than NORMAL, we can use the input diredctly:
    if set is true(1), add 0 to NORMAL, if it is false(0) add 1.
    this means add the negation of set to NORMAL
    */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL + !set);
}


static void window_close_callback(GLFWwindow* _window)
{
    checs_command_publish(WindowCloseCommand, NULL);
}


static void framebuffer_size_callback(GLFWwindow *const _window, int const _width, int const _height)
{
    checs_command_publish(FramebufferSizeCommand, (&(FramebufferSizeData){_width, _height}));
}


static void key_callback(GLFWwindow* _window, int key, int scancode, int action, int mods)
{
    checs_command_publish(KeyCommand, (&(KeyData){key, scancode, action, mods}));
    checs_event_publish(KeyEventData, KeyEvent, ((KeyEventData){key, scancode, action, mods}));
}


static void mouse_button_callback(GLFWwindow* _window, int button, int action, int mods)
{
    checs_command_publish(MouseButtonCommand, (&(MouseButtonData){button, action, mods}));
}


static void joystick_callback(int jid, int event)
{
    checs_command_publish(JoystickCommand, (&(JoystickData){jid, event}));
}


static void scroll_callback(GLFWwindow* _window, double const xoffset, double const yoffset)
{
    /* we dont care about xoffset scrolling, so we are leaving that out */
    checs_command_publish(ScrollCommand, (&(ScrollData){yoffset}));
}