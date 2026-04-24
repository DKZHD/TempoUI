#include "AnimationManager.h"

AnimationManager& AnimationManager::get()
{
    static AnimationManager instance;
    return instance;
}

void AnimationManager::update(float delta_time)
{
    int index = 0;
    for (auto& [uuid, animation] : animations_)
    {
        animation.update(delta_time);
        index++;
    }

    std::erase_if(animations_, [](const auto& pair)
    {
        return pair.second.is_complete_;
    });
}

AnimationHandle AnimationManager::add_animation(std::function<void(float)> func, float duration, AnimationType type, AnimDirection dir)
{
    AnimationHandle handle;
    animations_.emplace(handle.uuid.to_string(), Animation{ std::move(func), duration, type, dir });
    return handle;
}

namespace Animate
{
    AnimationHandle Start(std::function<void(float)> func, float duration, AnimationType type, AnimDirection dir)
    {
        return AnimationManager::get().add_animation(std::move(func), duration, type, dir);
    }
} // namespace Animate
