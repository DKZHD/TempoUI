#pragma once

enum class AnimationType : uint8_t
{
    SmoothStep,
    Perlin_SmoothStep,
};
enum class AnimDirection : uint8_t
{
    Forward,
    Backward,
};

class Animation
{
public:
    Animation(std::function<void(float)> callback, float duration, AnimationType type = AnimationType::Perlin_SmoothStep, AnimDirection dir = AnimDirection::Forward);
    ~Animation() = default;
    void update(float delta_time);
    void cancel();

    float time_elapsed_ = 0.f;
    float duration_ = 0.f;
    bool is_complete_ = false;
    AnimationType type_;
    AnimDirection dir_;
    std::function<void(float)> callback_;
};
