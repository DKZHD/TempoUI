#pragma once
#include "Elements/ElementBase/ElementBase.h"

class UIMesh;

namespace CustomFlags
{
    enum class TextValues : uint32_t // NOLINT
    {
        Text = 1 << 8,
        Color = 1 << 9,
        TextVertAlignment = 1 << 10,
        TextHoriAlignment = 1 << 11
    };
    inline uint32_t& operator|=(uint32_t& val, TextValues right)
    {
        val |= (uint32_t)right;
        return val;
    }
    inline bool operator&(uint32_t val, TextValues right)
    {
        return val & (uint32_t)right;
    }
} // namespace CustomFlags

enum class TextVerticalAlignment : uint8_t
{
    Top,
    Center,
    Bottom
};
enum class TextHorizontalAlignment : uint8_t
{
    Left,
    Center,
    Right
};

class TextConfig : public ElementConfig<TextConfig>
{
public:
    TextConfig();
    TextConfig& text(std::string text);
    TextConfig& color(glm::vec3 color);
    TextConfig& align_vert(TextVerticalAlignment alignment);
    TextConfig& align_hori(TextHorizontalAlignment alignment);
    [[nodiscard]] std::string text() const { return text_; }
    [[nodiscard]] glm::vec3 color() const { return color_; }
    [[nodiscard]] TextVerticalAlignment align_vert() const { return vert_alignment_; }
    [[nodiscard]] TextHorizontalAlignment align_hori() const { return hori_alignment_; }

private:
    std::string text_;
    glm::vec3 color_ = glm::vec3(0.f);
    TextVerticalAlignment vert_alignment_ = TextVerticalAlignment::Top;
    TextHorizontalAlignment hori_alignment_ = TextHorizontalAlignment::Left;
};

class Text : public ElementBase
{
public:
    Text(const TextConfig& _config);
    Text(const std::string& id, const std::string& text, glm::vec2 pos, glm::vec3 color = glm::vec3(0.f));
    [[nodiscard]] const std::string& get_text() const { return text_; }
    void update_text(const std::string& text);
    void set_should_wrap(bool should_wrap);
    void update_buffers() override;
    void update_color(glm::vec4 color);
    ~Text();

private:
    void self_theme_updated() override;
    void self_scale_updated(glm::vec2 new_scale) override;

    glm::vec2 current_scale = { 1.f, 1.f };
    glm::vec2 prev_scale = { 1.f, 1.f };
    bool should_wrap_ = true;
    glm::vec3 color_ = glm::vec3(0.f);
    std::string text_;
    std::string written_text;
    std::vector<UIMesh> meshes_;
    TextVerticalAlignment vert_alignment_ = TextVerticalAlignment::Top;
    TextHorizontalAlignment hori_alignment_ = TextHorizontalAlignment::Left;

    glm::vec2 last_parent_size_ = glm::vec2(-1.f);
};
