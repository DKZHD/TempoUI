#pragma once

#include "Elements/ElementBase/ElementBase.h"
#include "UIMesh/UIMesh.h"

namespace CustomFlags
{
    enum class ColorPickerValues : uint32_t // NOLINT
    {
        Selector_Background_Color = 1 << 8,
        Selector_Inner_Diameter = 1 << 9,
        Selector_Outer_Diameter = 1 << 10
    };

    inline uint32_t& operator|=(uint32_t& val, ColorPickerValues right)
    {
        val |= (uint32_t)right;
        return val;
    }
    inline bool operator&(uint32_t val, ColorPickerValues right)
    {
        return val & (uint32_t)right;
    }
} // namespace CustomFlags

class ColorPickerConfig : public ElementConfig<ColorPickerConfig>
{
public:
    ColorPickerConfig();
    ColorPickerConfig& selector_background_color(glm::vec4 color);
    ColorPickerConfig& selector_outer_diameter(float diameter);
    ColorPickerConfig& selector_inner_diameter(float diameter);
    [[nodiscard]] glm::vec4 selector_background_color() const { return selector_background_color_; };
    [[nodiscard]] float selector_outer_diameter() const { return selector_outer_diameter_; };
    [[nodiscard]] float selector_inner_diameter() const { return selector_inner_diameter_; };

private:
    glm::vec4 selector_background_color_ = glm::vec4(1.f);
    float selector_inner_diameter_ = 20.f;
    float selector_outer_diameter_ = 24.f;
};

class ColorPicker : public ElementBase
{
public:
    ColorPicker(const ColorPickerConfig& _config);
    ColorPicker(const std::string& id, glm::vec2 pos, glm::vec2 size);
    [[nodiscard]] glm::vec4 get_color() const;
    void set_selector_position(glm::vec2 relative_pos);
    void set_normalized_selector_position(glm::vec2 normalized_pos);

private:
    bool is_dragging = false;

    void update_buffers() override;
    void self_theme_updated() override;
    void self_update(float delta_time) override;
    void self_on_mouse_button(UI::MouseButton button, UI::Action action) override;
    std::array<UIMesh, 3> meshes_;

    float selector_inner_diameter_ = 20.f;
    float selector_outer_diameter_ = 24.f;
    glm::vec4 selector_background_color_ = glm::vec4(1.f);

    glm::vec2 normalized_position_{ 0.5, 0.5 };
};