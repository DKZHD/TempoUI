#pragma once
#include "AnimationManager/AnimationHandle.h"
#include "Elements/ElementBase/ElementBase.h"
#include "UIMesh/UIMesh.h"

namespace TempoUI
{

    namespace CustomFlags
    {
        enum class CheckboxValues : uint32_t // NOLINT
        {
            Check_Color = 1 << 8,
            Hover_Color = 1 << 9,
            Background_Color = 1 << 10,
        };

        inline uint32_t& operator|=(uint32_t& val, CheckboxValues right)
        {
            val |= (uint32_t)right;
            return val;
        }
        inline bool operator&(uint32_t val, CheckboxValues right)
        {
            return val & (uint32_t)right;
        }
    } // namespace CustomFlags

    class CheckboxConfig : public ElementConfig<CheckboxConfig>
    {
    public:
        CheckboxConfig();
        CheckboxConfig& default_state(bool _default);
        CheckboxConfig& check_color(glm::vec4 color);
        CheckboxConfig& background_color(glm::vec4 color);
        CheckboxConfig& hover_color(glm::vec4 color);
        [[nodiscard]] bool default_state() const { return default_; };
        [[nodiscard]] glm::vec4 check_color() const { return check_color_; }
        [[nodiscard]] glm::vec4 background_color() const { return background_color_; }
        [[nodiscard]] glm::vec4 hover_color() const { return hover_color_; }

    private:
        bool default_ = false;
        glm::vec4 check_color_ = glm::vec4(0.f, 0.f, 0.f, 1.f);
        glm::vec4 background_color_ = glm::vec4(1.f);
        glm::vec4 hover_color_ = glm::vec4(0.5f, 0.5f, 0.5f, 1.f);
    };

    class Checkbox : public ElementBase
    {
    public:
        Checkbox(const CheckboxConfig& _config);
        Checkbox(const std::string& id, glm::vec2 pos, glm::vec2 size, glm::vec4 radius = glm::vec4(5.f), bool default_state = false);
        void bind_variable(bool& value);
        [[nodiscard]] bool get_value() const;
        void set_value(bool new_value);

    private:
        void self_update(float delta_time) override;
        void self_on_mouse_button(UI::MouseButton button, UI::Action action) override;
        void self_theme_updated() override;
        void update_buffers() override;
        std::array<UIMesh, 2> meshes_;

        bool checked_;
        bool* attached_variable_ = nullptr;

        glm::vec4 check_color_ = glm::vec4(0.f, 0.f, 0.f, 1.f);
        glm::vec4 background_color_ = glm::vec4(1.f);
        glm::vec4 hover_color_ = glm::vec4(0.5f);
        AnimationHandle handle;
    };
} // namespace TempoUI
