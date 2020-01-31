#include "window.h"

static vec2ui16 size;
static int videoModeCount;
static GLFWvidmode const* modes;
static GLFWvidmode mode;
GLFWwindow* window;
GLFWmonitor* monitor;

void framebuffer_size_callback(GLFWwindow *const window, int const width, int const height);

void window_init(char const* const title)
{
    assert(glfwInit());

    monitor = glfwGetPrimaryMonitor();
    modes = glfwGetVideoModes(monitor, &videoModeCount);
    mode = modes[videoModeCount - 1];

    size = (vec2ui16){mode.width, mode.height};

    glfwWindowHint(GLFW_RED_BITS, mode.redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode.greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode.blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode.refreshRate);

    window = glfwCreateWindow(size.x, size.y, title, monitor, NULL);
    if (!window)
    {
        glfwTerminate();
        printf("Failed to construct window\n");
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwMakeContextCurrent(window);

    assert(glewInit() == GLEW_OK);
}


void window_terminate(void)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}

void framebuffer_size_callback(GLFWwindow *const window, int const width, int const height)
{
    glViewport(0, 0, width, height);
}