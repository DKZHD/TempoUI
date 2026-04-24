#include <GLFW/glfw3.h>
#include "Window/Window.h"

int main()
{
    Window::Initialize();
    Window window(1920, 1080, "Window");

    while (!glfwWindowShouldClose(window))
    {
        window.Run();
    }
    return 0;
}
