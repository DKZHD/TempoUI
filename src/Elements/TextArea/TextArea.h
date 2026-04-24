#pragma once
#include "Elements/ElementBase/ElementBase.h"
#include "Elements/Text/Text.h"
#include "UIMesh/UIMesh.h"

namespace CustomFlags
{
    enum class TextAreaValues : uint32_t // NOLINT
    {
        Text_Color = 1 << 8
    };

    inline uint32_t& operator|=(uint32_t& val, TextAreaValues right)
    {
        val |= (uint32_t)right;
        return val;
    }
    inline bool operator&(uint32_t val, TextAreaValues right)
    {
        return val & (uint32_t)right;
    }
} // namespace CustomFlags

class TextAreaConfig : public ElementConfig<TextAreaConfig>
{
public:
    TextAreaConfig();
    TextAreaConfig& text_color(glm::vec4 color);
    [[nodiscard]] glm::vec4 text_color() const { return text_color_; }

private:
    glm::vec4 text_color_ = glm::vec4(0.f, 0.f, 0.f, 1.f);
};

class TextArea : public ElementBase
{
public:
    TextArea(const TextAreaConfig& _config);
    TextArea(const std::string& id, glm::vec2 pos, glm::vec2 size);
    std::string get_text();
    ~TextArea() override = default;

private:
    void recalculate_cursor();
    void self_on_char(char character) override;
    void self_on_key(UI::Key key, UI::Action action) override;
    void self_on_mouse_button(UI::MouseButton button, UI::Action action) override;
    void self_update(float dt) override;
    void self_theme_updated() override;
    void self_scale_updated(glm::vec2 new_scale) override;
    void update_buffers() override;
    void reset_cursor();
    void recalculate_layout();
    void update_texts();

    UIMesh cursor_;

    std::vector<int> text_child_indexes_;
    std::vector<int> line_lengths_;
    std::vector<std::string> lines_of_text_;
    std::vector<Text*> texts_;

    unsigned int cursor_index_ = 0;

    std::string raw_text_;
    bool is_focused_ = false;
    float blink_timer_ = 0.f;
    glm::vec4 current_color_ = glm::vec4(0.f, 0.f, 0.f, 1.f);

    glm::vec2 prev_size = glm::vec2(-1);
};
