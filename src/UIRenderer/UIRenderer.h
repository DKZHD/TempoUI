#pragma once

class ElementBase;
struct Vertex;
class Shader;
class Canvas;
class IRenderer;

struct Batch
{
    unsigned int start_index = 0;
    unsigned int index_count = 0;
    bool use_scissor = false;
    glm::vec4 scissor_area = glm::vec4(0.f);
};

class UIRenderer
{
public:
    UIRenderer(const char* theme_data = nullptr);
    ~UIRenderer();

    void init(float width, float height);
    bool draw(float delta_time);
    void update(float delta_time);
    void render();

    void add_image(const std::string& path);
    void add_image(const std::string& name, const std::string& path);
    void add_image_from_memory(const std::string& name, unsigned char* data, int length);
    void add_font(const std::string& name, const std::string& path, unsigned int font_size);
    void add_font_from_memory(const std::string& name, unsigned char* data, long length, unsigned int font_size);

    template <typename T, typename... Args>
    T* add_element(Args&&... args);

    template <typename T>
    T* get_element(const std::string& id);

    void bind_layout_callback(std::function<void()> func);
    void load_layout(const std::string& layout_name);

    void remove_element_from_canvas(const std::string& id);

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Canvas> canvas;

    std::unique_ptr<IRenderer> backend_renderer;
    std::unique_ptr<std::function<void()>> layout_callback_ = nullptr;

    ElementBase* last_hovered = nullptr;
    std::vector<Batch> render_batches_;
    void process_elements(ElementBase* element, glm::vec4 active_clipping_area);
};

#include "UIRenderer.ipp" // IWYU pragma: keep
