#include "../../engine.h"


static void window_close_callback(GLFWwindow* window);
static void framebuffer_size_callback(GLFWwindow *const window, int const width, int const height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
static void joystick_callback(int jid, int event);
static void scroll_callback(GLFWwindow* window, double const xoffset, double const yoffset);


void inputManager_init(void)
{
	glfwSetWindowCloseCallback(window, window_close_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetJoystickCallback(joystick_callback);
    glfwSetScrollCallback(window, scroll_callback);
}


static void window_close_callback(GLFWwindow* window)
{
    checs_command_publish(WindowCloseCommand, NULL);
}


static void framebuffer_size_callback(GLFWwindow *const window, int const width, int const height)
{
    checs_command_publish(FramebufferSizeCommand, (&(FramebufferSizeData){width, height}));
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    checs_command_publish(KeyCommand, (&(KeyData){key, scancode, action, mods}));
    checs_event_publish(KeyEventData, KeyEvent, ((KeyEventData){key, scancode, action, mods}));
}


static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    checs_command_publish(MouseButtonCommand, (&(MouseButtonData){button, action, mods}));
}


static void joystick_callback(int jid, int event)
{
    checs_command_publish(JoystickCommand, (&(JoystickData){jid, event}));
}


static void scroll_callback(GLFWwindow* window, double const xoffset, double const yoffset)
{
    /* we dont care about xoffset scrolling, so we are leaving that out */
    checs_command_publish(ScrollCommand, (&(ScrollData){yoffset}));
}