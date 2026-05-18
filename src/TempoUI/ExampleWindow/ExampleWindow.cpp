#include "ExampleWindow.h"
#include <GLFW/glfw3.h>

// IWYU pragma: begin_keep
#include "Elements/BorderedBox/BorderedBox.h"
#include "Elements/Button/Button.h"
#include "Elements/Combobox/Combobox.h"
#include "Elements/HorizontalBox/HorizontalBox.h"
#include "Elements/Image/Image.h"
#include "Elements/InputBox/InputBox.h"
#include "Elements/ProgressBar/ProgressBar.h"
#include "Elements/ScrollBox/ScrollBox.h"
#include "Elements/Slider/Slider.h"
#include "Elements/Text/Text.h"
#include "Elements/TextArea/TextArea.h"
#include "Elements/VerticalBox/VerticalBox.h"
#include "Elements/Checkbox/Checkbox.h"
#include "Elements/ToggleSlider/ToggleSlider.h"
#include "Elements/ColorPicker/ColorPicker.h"
#include "Elements/Wrapbox/Wrapbox.h"
#include "Elements/GraphView/GraphView.h"
// IWYU pragma: end_keep

#include "ThemeManager/ThemeManager.h"
#include "UIRenderer/UIRenderer.h"
#include "FileReader/FileReader.h"
#include <fstream>

namespace
{
    float test = 0.5f;

} // namespace

void Window::Initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    GLFWwindow* temp = glfwCreateWindow(1, 1, "Bootstrap", nullptr, nullptr);
    glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
    glfwMakeContextCurrent(temp);

    if (gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)) == 0)
    {
        throw std::runtime_error("Failed to initialize GLAD");
    }
    glfwDestroyWindow(temp);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
}

Window::Window(int width, int height, const char* title) : window(glfwCreateWindow(width, height, title, nullptr, nullptr))
{
    if (window == nullptr)
        throw std::runtime_error("Failed to create GLFW window");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glViewport(0, 0, width, height);

    ui_renderer = std::make_unique<TempoUI::UIRenderer>(TempoUI::FileReader::read_File("resources/themes/theme.json").c_str());
    ui_renderer->init(static_cast<float>(width), static_cast<float>(height));

    TempoUI::UI::GLFW::init_input(window);

    std::ifstream font_file("resources/fonts/RobotoMono-Light.ttf", std::ios::binary | std::ios::ate);
    std::streamsize size = font_file.tellg();
    font_file.seekg(0, std::ios::beg);
    std::vector<unsigned char> font_buffer(size);
    font_file.read(reinterpret_cast<char*>(font_buffer.data()), size);

    glfwSetWindowUserPointer(*this, this);

    glfwSetFramebufferSizeCallback(*this, [](GLFWwindow* window, int x, int y)
    {
        if (Window* self = static_cast<Window*>(glfwGetWindowUserPointer(window)))
        {
            glm::vec2 new_size = { x, y };
            glm::vec2 original_size = { 1920, 1080 };
            self->ui_renderer->get_element<TempoUI::Canvas>("Screen")->scale_updated(new_size / original_size);
        }
        glViewport(0, 0, x, y);
    });

    ui_renderer->add_font_from_memory("default_font", font_buffer.data(), (long)font_buffer.size(), 36);

    ui_renderer->bind_layout_callback([this]()
    {
        test = 0.f;                                                                       // NOLINT
        ui_renderer->get_element<TempoUI::ProgressBar>("PB")->bind_variable(test, 100.f); // NOLINT
        ui_renderer->get_element<TempoUI::GraphView>("View")->bind_variable(&test);
    });
    ui_renderer->load_layout("layout.json");
}

void Window::process_input()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
    {
        TempoUI::ThemeManager::get().load_theme(TempoUI::FileReader::read_File("../theme.json"));
        ui_renderer->get_element<TempoUI::Canvas>("Screen")->theme_updated();
    }
    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
    {
        TempoUI::ThemeManager::get().load_theme(TempoUI::FileReader::read_File("../theme.json"));
        ui_renderer->get_element<TempoUI::Canvas>("Screen")->theme_updated();
        ui_renderer->remove_element_from_canvas("Layout_Canvas");
        ui_renderer->load_layout("layout.json");

        int x = 0;
        int y = 0;
        glfwGetFramebufferSize(window, &x, &y);
        glm::vec2 new_size = { x, y };
        glm::vec2 original_size = { 1920, 1080 };
        ui_renderer->get_element<TempoUI::Canvas>("Screen")->scale_updated(new_size / original_size);
    }
}

Window::~Window()
{
    if (window != nullptr)
    {
        glfwDestroyWindow(window);
    }
}

void Window::Run()
{
    float this_frame = (float)glfwGetTime();
    delta_time = this_frame - last_frame;
    last_frame = this_frame;

    test += delta_time * 10.f;
    if (test > 100.f)
        test = 0.f;

    glfwWaitEventsTimeout(0.008);
    process_input();

    if (ui_renderer->draw(delta_time))
    {
        glfwSwapBuffers(*this);
    }
}