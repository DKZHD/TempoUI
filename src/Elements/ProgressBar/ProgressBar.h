#pragma once
#include "Elements/ElementBase/ElementBase.h"
#include "AnimationManager/AnimationHandle.h"
#include "UIMesh/UIMesh.h"

namespace CustomFlags
{
    enum class ProgressBarValues : uint32_t // NOLINT
    {
        Inset = 1 << 8,
        Color = 1 << 9,
        Border_Color = 1 << 10,
    };

    inline uint32_t& operator|=(uint32_t& val, ProgressBarValues right)
    {
        val |= (uint32_t)right;
        return val;
    }
    inline bool operator&(uint32_t val, ProgressBarValues right)
    {
        return val & (uint32_t)right;
    }
} // namespace CustomFlags

class ProgressBarConfig : public ElementConfig<ProgressBarConfig>
{
public:
    ProgressBarConfig();
    ProgressBarConfig& percent(float _percent)
    {
        this->percent_ = _percent;
        return *this;
    }
    ProgressBarConfig& max(float _max)
    {
        this->max_ = _max;
        return *this;
    }
    ProgressBarConfig& inset(float _inset)
    {
        custom_values |= CustomFlags::ProgressBarValues::Inset;
        this->inset_ = _inset;
        return *this;
    }
    ProgressBarConfig& border_color(glm::vec4 _color)
    {
        custom_values |= CustomFlags::ProgressBarValues::Border_Color;
        border_color_ = _color;
        return *this;
    }
    ProgressBarConfig& color(glm::vec4 _color)
    {
        custom_values |= CustomFlags::ProgressBarValues::Color;
        color_ = _color;
        return *this;
    }

    [[nodiscard]] float percent() const { return percent_; }
    [[nodiscard]] float max() const { return max_; }
    [[nodiscard]] float inset() const { return inset_; }
    [[nodiscard]] glm::vec4 border_color() const { return border_color_; }
    [[nodiscard]] glm::vec4 color() const { return color_; }

private:
    float percent_ = 1.f;
    float max_ = 1.f;
    float inset_ = 0.f;
    glm::vec4 color_ = glm::vec4(1.f, 1.f, 1.f, 1.f);
    glm::vec4 border_color_ = glm::vec4(0.f, 0.f, 0.f, 1.f);
};

class ProgressBar : public ElementBase
{
public:
    ProgressBar(const ProgressBarConfig& _config);
    ProgressBar(const std::string& id, glm::vec2 pos, glm::vec2 size, float max, float inset = 0.f,
        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), glm::vec4 borderColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4 radius = glm::vec4(0.f));
    ~ProgressBar() override;

    void set_percent(float percent);
    [[nodiscard]] float get_percent() const;
    void bind_variable(float& value, float max);

private:
    void self_update(float dt) override;
    void self_theme_updated() override;
    void update_buffers() override;

    float target_percent = 0.f;
    float percent_ = 1.f;
    float* attached_variable_ = nullptr;
    float max_ = 1.f;

    std::array<UIMesh, 2> meshes_;
    float inset_ = 0.f;
    AnimationHandle handle;
};
