#pragma once
#include "Elements/ElementBase/ElementBase.h"
#include "UIMesh/UIMesh.h"

namespace CustomFlags
{
    enum class SliderValues : uint32_t // NOLINT
    {
        Min = 1 << 8,
        Max = 1 << 9,
        Color = 1 << 10,
        Slider_Size = 1 << 11
    };

    inline uint32_t& operator|=(uint32_t& val, SliderValues right)
    {
        val |= (uint32_t)right;
        return val;
    }
    inline bool operator&(uint32_t val, SliderValues right)
    {
        return val & (uint32_t)right;
    }
} // namespace CustomFlags

class SliderConfig : public ElementConfig<SliderConfig>
{
public:
    SliderConfig();
    SliderConfig& min(float min);
    SliderConfig& max(float max);
    SliderConfig& slider_size(float size);
    SliderConfig& color(glm::vec4 color);
    [[nodiscard]] float min() const { return min_; }
    [[nodiscard]] float max() const { return max_; }
    [[nodiscard]] float slider_size() const { return slider_size_; }
    [[nodiscard]] glm::vec4 color() const { return color_; }

private:
    float min_ = 0.f;
    float max_ = 1.f;
    float slider_size_ = 10.f;
    glm::vec4 color_ = glm::vec4(1.f);
};

class Slider : public ElementBase
{
public:
    Slider(const SliderConfig& _config);
    Slider(const std::string& id, glm::vec2 pos, glm::vec2 size, glm::vec4 color = glm::vec4(1.f), glm::vec4 radius = glm::vec4(0.f), float min = 0.f, float max = 1.f);
    ~Slider();

    void set_value(float value);
    float get_value() const;
    void bind_variable(float& variable);

private:
    void self_on_scroll(float yOffset) override;
    void self_on_mouse_button(UI::MouseButton button, UI::Action action) override;
    void self_on_char(char character) override;
    void self_update(float dt) override;
    void update_buffers() override;
    void self_theme_updated() override;

    bool is_dragging_ = false;
    float* attached_variable_ = nullptr;

    float value_ = 0.5f;
    float slider_size = 10.f;
    std::array<UIMesh, 2> meshes_;
    float min_ = 0.f;
    float max_ = 1.f;

    glm::vec4 color_ = glm::vec4(1.f);
};
