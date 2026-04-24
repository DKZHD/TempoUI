#pragma once
#include "AnimationManager/AnimationHandle.h"
#include "Elements/ElementBase/ElementBase.h"

namespace CustomFlags
{
    enum class ButtonValues : uint32_t // NOLINT
    {
        Default_Color = 1 << 8,
        Hover_Color = 1 << 9,
        Clicked_Color = 1 << 10,
    };

    inline uint32_t& operator|=(uint32_t& val, ButtonValues right)
    {
        val |= (uint32_t)right;
        return val;
    }
    inline bool operator&(uint32_t val, ButtonValues right)
    {
        return val & (uint32_t)right;
    }
} // namespace CustomFlags

class ButtonConfig : public ElementConfig<ButtonConfig>
{
public:
    ButtonConfig();
    // Also sets hover & clicked color. Use separate functions for setting hover & clicked after this function (optional)
    ButtonConfig& color(glm::vec4 color)
    {
        custom_values |= CustomFlags::ButtonValues::Default_Color;
        custom_values |= CustomFlags::ButtonValues::Hover_Color;
        custom_values |= CustomFlags::ButtonValues::Clicked_Color;

        this->default_color_ = color;
        this->hover_color_ = color * 1.2f;
        this->clicked_color_ = glm::vec4(1.f - glm::vec3(color), 0.7f);
        return *this;
    }

    // If color is set after, this will get overridden
    ButtonConfig& hover_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::ButtonValues::Hover_Color;
        this->hover_color_ = color;
        return *this;
    }
    // If color is set after, this will get overridden
    ButtonConfig& clicked_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::ButtonValues::Clicked_Color;
        this->clicked_color_ = color;
        return *this;
    }

    [[nodiscard]] glm::vec4 color() const { return default_color_; }
    [[nodiscard]] glm::vec4 hover_color() const { return hover_color_; }
    [[nodiscard]] glm::vec4 clicked_color() const { return clicked_color_; }

protected:
    glm::vec4 default_color_ = glm::vec4(1.f);
    glm::vec4 hover_color_ = glm::vec4(1.f);
    glm::vec4 clicked_color_ = glm::vec4(1.f);
};

class UIMesh;

class Button : public ElementBase
{
public:
    Button(const ButtonConfig& _config);
    Button(const std::string& id, glm::vec2 pos, glm::vec2 size, glm::vec4 color = glm::vec4(1.f), glm::vec4 radius = glm::vec4(0.f));
    ~Button() override = default;

    void set_on_click(std::function<void()> func);
    void trigger_on_click();

    void update_color(glm::vec4 color);
    void update_radius(glm::vec4 radius);

private:
    void self_on_mouse_button(UI::MouseButton button, UI::Action action) override;
    void self_update(float dt) override;
    void on_hover() override;
    void on_unhover() override;
    void update_buffers() override;
    void self_theme_updated() override;

    glm::vec4 default_color_ = glm::vec4(1.f);
    glm::vec4 hover_color_ = glm::vec4(1.f);
    glm::vec4 clicked_color_ = glm::vec4(1.f);

    std::unique_ptr<UIMesh> button_mesh;
    std::function<void()> on_click;

    AnimationHandle handle;
};
