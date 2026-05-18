#pragma once

#include "Elements/ElementBase/ElementBase.h"

namespace TempoUI
{
    class ScrollBox;
    class Button;
    class Text;

    namespace CustomFlags
    {
        enum class ComboboxValues : uint32_t // NOLINT
        {
            Main_Color = 1 << 8,
            Sub_Color = 1 << 9,
            Spacing = 1 << 10
        };

        inline uint32_t& operator|=(uint32_t& val, ComboboxValues right)
        {
            val |= (uint32_t)right;
            return val;
        }
        inline bool operator&(uint32_t& val, ComboboxValues right)
        {
            return val & (uint32_t)right;
        }
    } // namespace CustomFlags

    class ComboboxConfig : public ElementConfig<ComboboxConfig>
    {
    public:
        ComboboxConfig();
        ComboboxConfig& spacing(float spacing);
        ComboboxConfig& main_color(glm::vec4 color);
        ComboboxConfig& sub_color(glm::vec4 color);
        [[nodiscard]] float spacing() const { return spacing_; }
        [[nodiscard]] glm::vec4 main_color() const { return main_color_; }
        [[nodiscard]] glm::vec4 sub_color() const { return sub_color_; }

    private:
        float spacing_ = 0.f;
        glm::vec4 main_color_ = glm::vec4(1.f);
        glm::vec4 sub_color_ = glm::vec4(0.8, 0.8, 0.8, 1.0);
    };

    class Combobox : public ElementBase
    {
    public:
        Combobox(const ComboboxConfig& _config);
        Combobox(const std::string& id, glm::vec2 pos, glm::vec2 size, float spacing = 0.f);
        void add_option(const std::string& option);
        void add_multiple_options(std::initializer_list<std::string> options);

        int get_value_as_int();
        std::string get_value();

    private:
        bool isHovered() override;
        void self_on_scroll(float y_offset) override;
        void self_on_mouse_button(UI::MouseButton button, UI::Action action) override;
        void self_theme_updated() override;
        void update_buffers() override;

        int selected_index = 0;
        bool expanded = false;
        ScrollBox* scroll_box = nullptr;
        Button* button = nullptr;
        Text* main_text_ = nullptr;
        float spacing_ = 0.f;

        glm::vec4 sub_color_ = glm::vec4(0.8, 0.8, 0.8, 1.0);
    };
} // namespace TempoUI
