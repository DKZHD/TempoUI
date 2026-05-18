#pragma once
#include "Elements/ElementBase/ElementBase.h"
#include "Elements/Text/Text.h"
#include "UIInputHandler/UIInputHandler.h"
#include "UIMesh/UIMesh.h"

namespace TempoUI
{
    namespace CustomFlags
    {
        enum class InputBoxValues : uint32_t // NOLINT
        {
            Text_Color = 1 << 8,
            Background_Color = 1 << 9,
            Cursor_Color = 1 << 10
        };

        inline uint32_t& operator|=(uint32_t& val, InputBoxValues right)
        {
            val |= (uint32_t)right;
            return val;
        }
        inline bool operator&(uint32_t val, InputBoxValues right)
        {
            return val & (uint32_t)right;
        }
    } // namespace CustomFlags

    class InputBoxConfig : public ElementConfig<InputBoxConfig>
    {
    public:
        InputBoxConfig();
        InputBoxConfig& background_color(glm::vec4 color);
        InputBoxConfig& cursor_color(glm::vec4 color);
        InputBoxConfig& text_color(glm::vec4 color);
        [[nodiscard]] glm::vec4 background_color() const { return background_color_; };
        [[nodiscard]] glm::vec4 cursor_color() const { return cursor_color_; };
        [[nodiscard]] glm::vec4 text_color() const { return text_color_; };

    private:
        glm::vec4 background_color_ = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
        glm::vec4 cursor_color_ = glm::vec4(0.7f, 0.7f, 0.7f, 1.f);
        glm::vec4 text_color_ = glm::vec4(1.f);
    };

    class InputBox : public ElementBase
    {
    public:
        InputBox(const InputBoxConfig& _config);
        InputBox(const std::string& id, glm::vec2 pos, glm::vec2 size);
        ~InputBox() override;

        std::string get_text();
        void on_enter(std::function<void(std::string)> func);

        // Template only accepts chars, any other type will cause a compiler error!
        template <typename... Args>
            requires(std::same_as<Args, char> && ...)
        void limit_input(Args... args)
        {
            (chars_.set(static_cast<unsigned char>(args)), ...);
        }

    private:
        void self_on_mouse_button(UI::MouseButton button, UI::Action action) override;
        void self_on_char(char character) override;
        void self_on_key(UI::Key key, UI::Action action) override;
        void self_update(float dt) override;
        void self_theme_updated() override;
        void update_buffers() override;

        [[nodiscard]] bool is_allowed(char c) const;
        void reset_cursor();

        unsigned int cursor_index_ = 0;
        std::array<UIMesh, 2> meshes_;

        bool is_focused_ = false;
        float blink_timer_ = 0.0f;
        std::bitset<256> chars_;

        Text* text_ = nullptr;
        std::unique_ptr<std::function<void(std::string)>> callback_;

        glm::vec4 background_color_ = glm::vec4(0.4f, 0.4f, 0.4f, 1.0f);
        glm::vec4 cursor_color_ = glm::vec4(0.7f, 0.7f, 0.7f, 1.f);
        glm::vec4 text_color_ = glm::vec4(1.f);
    };
} // namespace TempoUI
