#include "Animation.h"

namespace
{
    float get_value(float time_elapsed, float duration, TempoUI::AnimationType type, TempoUI::AnimDirection dir)
    {
        float value = std::clamp(time_elapsed / duration, 0.f, 1.f);
        if (dir == TempoUI::AnimDirection::Backward)
        {
            value = 1.f - value;
        }
        switch (type)
        {
        case TempoUI::AnimationType::SmoothStep:
        {
            return (float)(value * value * (3.0 - 2.0 * value));
        }
        break;
        case TempoUI::AnimationType::Perlin_SmoothStep:
            return (float)(value * value * value * (value * (6.0 * value - 15.0) + 10.0));
            break;
        }
        return 0.f;
    }
} // namespace

void TempoUI::Animation::update(float delta_time)
{
    time_elapsed_ += delta_time;

    if (time_elapsed_ >= duration_)
    {
        time_elapsed_ = duration_;
        is_complete_ = true;
    }

    float value = get_value(time_elapsed_, duration_, type_, dir_);
    if (callback_)
    {
        callback_(value);
    }
}

void TempoUI::Animation::cancel()
{
    is_complete_ = true;
    time_elapsed_ = duration_;
    callback_ = nullptr;
}

TempoUI::Animation::Animation(std::function<void(float)> callback, float duration, AnimationType type, AnimDirection dir) : callback_(std::move(callback)), duration_(duration), type_(type), dir_(dir)
{
}
