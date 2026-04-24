#pragma once
#include "UIInputHandler/UIInputHandler.h"

class EventManager;
class JsonWriter;
struct Vertex;

enum ElementState : uint8_t
{
    VISIBLE = 1 << 0,
    IS_HOVERED = 1 << 1,
    CLICKABLE = 1 << 2,
    HOVERABLE = 1 << 3,
    IS_CONTAINER = 1 << 4,
    CLIP_CHILDREN = 1 << 5,
    IS_OVERLAY = 1 << 6
};

namespace CustomFlags
{
    enum ElementConfigCustom : uint8_t
    {
        Size = 1 << 0,
        Radius = 1 << 1,
        Flex = 1 << 2,
    };
}

template <typename Derived>
class ElementConfig
{
public:
    ElementConfig() = default;

    Derived& id(std::string id);
    Derived& position(glm::vec2 pos);
    Derived& size(glm::vec2 size);
    Derived& radius(glm::vec4 radius);
    Derived& flex(int flex);

    [[nodiscard]] const std::string& id() const { return id_; }
    [[nodiscard]] glm::vec2 size() const { return size_; }
    [[nodiscard]] glm::vec2 position() const { return pos_; }
    [[nodiscard]] glm::vec4 radius() const { return radius_; }
    [[nodiscard]] int flex() const { return flex_; }
    [[nodiscard]] uint32_t get_flags() const { return custom_values; }

protected:
    uint32_t custom_values = 0;
    std::string id_;
    glm::vec2 pos_ = glm::vec2(0.f);
    glm::vec2 size_ = glm::vec2(0.f);
    glm::vec4 radius_ = glm::vec4(0.f, 0.f, 0.f, 0.f);
    int flex_ = 0;
};

struct ElementInfo
{
    uint8_t state = VISIBLE;
    glm::vec2 localPosition = glm::vec2(0.0f, 0.0f);
    glm::vec2 size = glm::vec2(0.0f, 0.0f);
    glm::vec4 radius = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
};

class ElementBase
{
public:
    ElementBase(std::string id, uint32_t flags = UINT32_MAX, int flex = 0);
    virtual ~ElementBase();

    template <typename T>
    T* add_child(std::unique_ptr<T> element);
    template <typename T, typename... Args>
    T* create_child(Args&&... args);
    template <typename T>
    T* get_element(std::string_view id);

    void remove_element(std::string_view id);

    void update_absolute_position();
    void set_local_position(glm::vec2 position);
    void set_size(glm::vec2 size);

    [[nodiscard]] ElementBase* get_parent() const;
    [[nodiscard]] glm::vec2 get_position() const;
    [[nodiscard]] const glm::vec2& get_local_position() const;
    [[nodiscard]] glm::vec2 get_size() const;
    [[nodiscard]] int get_flex() const { return flex_; }

    [[nodiscard]] std::vector<Vertex>& get_vertices();
    [[nodiscard]] const std::vector<unsigned int>& get_indices() const;

    std::vector<std::unique_ptr<ElementBase>>& get_children();

    virtual bool isHovered();
    ElementBase* get_hovered_element();
    ElementBase* get_hovered_overlay();

    virtual void render();

    void set_on_hover(std::function<void()> func);
    void set_on_unhover(std::function<void()> func);
    virtual void on_hover();
    virtual void on_unhover();

    virtual void on_key(UI::Key key, UI::Action action);
    virtual void on_mouse_button(UI::MouseButton button, UI::Action action);
    virtual void on_char(char character);
    virtual void on_scroll(float yOffset);
    virtual void update(float dt);
    virtual void update_buffers() {}
    virtual void theme_updated();
    virtual void scale_updated(glm::vec2 new_scale);

    virtual void serialize(JsonWriter& writer);
    std::unique_ptr<ElementBase> release_hovered_element();

    [[nodiscard]] bool get_is_dirty() const;

    [[nodiscard]] bool check_state(ElementState state) const;
    void toggle_state(ElementState state);
    void set_state(ElementState state, bool value);

    void mark_dirty();
    void clear_dirty();
    std::string ID;

protected:
    virtual void self_on_key(UI::Key key, UI::Action action) {}
    virtual void self_on_mouse_button(UI::MouseButton button, UI::Action action) {}
    virtual void self_on_char(char character) {}
    virtual void self_update(float dt) {}
    virtual void self_on_scroll(float yOffset) {}
    virtual void self_theme_updated() {}
    virtual void self_scale_updated(glm::vec2 new_scale) {}
    virtual void self_serialize(JsonWriter& writer) {};

    ElementBase* parent = nullptr;
    std::vector<std::unique_ptr<ElementBase>> children;
    ElementInfo config;

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    bool is_dirty = true;
    glm::vec2 absolute_position = glm::vec2(0.f, 0.f);

    std::unique_ptr<std::function<void()>> on_hover_func = nullptr;
    std::unique_ptr<std::function<void()>> on_unhover_func = nullptr;

    uint32_t custom_values = 0;
    int flex_ = 0;
    glm::vec2 global_scale_ = { 1.f, 1.f };

private:
    glm::vec2 original_size = glm::vec2(FLT_MAX);
};

#include "ElementBase.ipp" // IWYU pragma: keep