#pragma once
#include "Animation/Animation.h"
#include "AnimationHandle.h"

class AnimationManager
{
public:
    static AnimationManager& get();
    void update(float delta_time);
    AnimationHandle add_animation(std::function<void(float)> func, float duration, AnimationType type, AnimDirection dir = AnimDirection::Forward);

private:
    friend AnimationHandle;

    AnimationManager() = default;
    ~AnimationManager() = default;

    std::unordered_map<std::string, Animation> animations_;
};

namespace Animate
{
    AnimationHandle Start(std::function<void(float)> func, float duration, AnimationType type, AnimDirection dir = AnimDirection::Forward);
}