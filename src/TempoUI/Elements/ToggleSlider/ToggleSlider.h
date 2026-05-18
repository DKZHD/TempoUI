#pragma once
#include "Elements/ElementBase/ElementBase.h"
#include "UIMesh/UIMesh.h"
#include "AnimationManager/AnimationHandle.h"

namespace TempoUI
{
    class BorderedBox;
    class Button;

    namespace CustomFlags
    {
        enum class ToggleSliderValues : uint32_t // NOLINT
        {
            Slider_Color = 1 << 8,
            Hover_Color = 1 << 9,
            Background_Color = 1 << 10
        };
        inline uint32_t& operator|=(uint32_t& val, ToggleSliderValues right)
        {
            val |= (uint32_t)right;
            return val;
        }
        inline bool operator&(uint32_t val, ToggleSliderValues right)
        {
            return val & (uint32_t)right;
        }
    } // namespace CustomFlags

    class ToggleSliderConfig : public ElementConfig<ToggleSliderConfig>
    {
    public:
        ToggleSliderConfig();
        ToggleSliderConfig& slider_color(glm::vec4 color);
        ToggleSliderConfig& hover_color(glm::vec4 color);
        ToggleSliderConfig& background_color(glm::vec4 color);
        [[nodiscard]] glm::vec4 slider_color() const { return slider_color_; }
        [[nodiscard]] glm::vec4 background_color() const { return background_color_; }
        [[nodiscard]] glm::vec4 hover_color() const { return hover_color_; }

    private:
        glm::vec4 slider_color_ = glm::vec4(0.f, 0.f, 0.f, 1.f);
        glm::vec4 hover_color_ = glm::vec4(0.2f, 0.2f, 0.2f, 1.f);
        glm::vec4 background_color_ = glm::vec4(1.f);
    };

    class ToggleSlider : public ElementBase
    {
    public:
        ToggleSlider(const ToggleSliderConfig& _config);
        ToggleSlider(const std::string& id, glm::vec2 pos, glm::vec2 size, glm::vec4 color = glm::vec4(0.f, 0.f, 0.f, 1.f));
        void bind_variable(bool& var);
        [[nodiscard]] bool get_value() const { return toggled; };

    private:
        void update_buffers() override;
        void self_theme_updated() override;
        void self_scale_updated(glm::vec2 new_scale) override;
        void self_update(float delta_time) override;
        void self_on_mouse_button(UI::MouseButton button, UI::Action action) override;
        BorderedBox* bordered_box = nullptr;
        std::array<UIMesh, 2> meshes;

        bool toggled = false;
        bool* bound_variable = nullptr;
        AnimationHandle handle;

        glm::vec4 slider_color_ = glm::vec4(0.4, 0.4, 0.4, 1.0);
        glm::vec4 hover_color_ = glm::vec4(0.6, 0.6, 0.6, 1.0);
        glm::vec4 background_color_ = glm::vec4(1.0);
    };
} // namespace TempoUI
