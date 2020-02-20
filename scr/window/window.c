#include "window.h"

static uint16_t width, height;
static GLFWmonitor *monitor;
static bool fullscreen;
GLFWwindow* window;


void window_init(char const* const title, uint16_t const n_width, uint16_t const n_height)
{
    assert(glfwInit());
    monitor = glfwGetPrimaryMonitor();
    width = n_width;
    height = n_height;

    assert(window = glfwCreateWindow(width, height, title, NULL, NULL));

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //enables vertical sync

    assert(glewInit() == GLEW_OK);

    commands_init();


    igCreateContext(NULL);
    ImGuiIO *io = igGetIO();
    ImGui_ImplGlfw_InitForOpenGL(window, true); 
}


void window_terminate(void)
{
    glfwDestroyWindow(window);
    glfwTerminate();
}


void window_fullscreen_set(bool const set)
{
    if (fullscreen = set)
    {
        int videoModeCount;
        GLFWvidmode const *const mode = &glfwGetVideoModes(monitor, &videoModeCount)[videoModeCount - 1];
        width = mode->width;
        height = mode->height;
        glfwSetWindowMonitor(window, monitor, 0, 0, width, height, mode->refreshRate);
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