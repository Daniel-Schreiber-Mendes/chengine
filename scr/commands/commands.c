#include "commands.h"


//acts as a layer to convert callbacks of glfw to checs commands


static void window_close_callback(GLFWwindow* _window)
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
}


static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	checs_command_publish(MouseButtonCommand, (&(MouseButtonData){button, action, mods}));
}


static void joystick_callback(int jid, int event)
{
	checs_command_publish(JoystickCommand, (&(JoystickData){jid, event}));
}

void commands_init(void)
{
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetJoystickCallback(joystick_callback);
}