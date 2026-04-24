#pragma once

class UIRenderer;
struct GLFWwindow;

class Window
{
public:
    static void Initialize();

    Window(int width, int height, const char* title);
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    void process_input();

    void Run();
    operator GLFWwindow*() const
    {
        return window;
    }

private:
    float last_frame = 0.f;
    float delta_time = 0.f;
    GLFWwindow* window = nullptr;
    std::unique_ptr<UIRenderer> ui_renderer;
};
