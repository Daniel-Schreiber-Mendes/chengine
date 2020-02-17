#include "window.h"

static uint16_t width, height;
static GLFWmonitor *monitor;
GLFWwindow* window;


void window_init(char const* const title)
{
    assert(glfwInit());

    monitor = glfwGetPrimaryMonitor();

    {
        int videoModeCount;
        GLFWvidmode const *const modes = glfwGetVideoModes(monitor, &videoModeCount);
        GLFWvidmode const mode = modes[videoModeCount - 1];
        width = mode.width;
        height = mode.height;

        glfwWindowHint(GLFW_RED_BITS, mode.redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode.greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode.blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode.refreshRate);
    }

    assert(window = glfwCreateWindow(width, height, title, monitor, NULL));

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); //enables vertical sync

    assert(glewInit() == GLEW_OK);
}


void window_terminate(void)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}