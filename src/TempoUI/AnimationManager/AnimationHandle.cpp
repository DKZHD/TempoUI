#include "AnimationHandle.h"
#include "AnimationManager.h"

namespace TempoUI
{
    Animation* AnimationHandle::anim() const
    {
        auto& animations = AnimationManager::get().animations_;
        auto it = animations.find(uuid.to_string());

        if (it != animations.end())
            return &it->second;

        return nullptr;
    }

    AnimationHandle::~AnimationHandle()
    {
        Animation* anim_ = anim(); // NOLINT
        if (anim_)
        {
            anim_->cancel();
        }
    }

    bool AnimationHandle::is_valid() const
    {
        return (anim() != nullptr);
    }

    AnimationHandle& AnimationHandle::operator=(AnimationHandle&& other) noexcept
    {
        if (Animation* anim = this->anim())
            anim->cancel();

        this->uuid = other.uuid;
        other.uuid = UUID::null();
        return *this;
    }

    AnimationHandle::AnimationHandle(AnimationHandle&& other) noexcept : uuid(other.uuid)
    {
        other.uuid = UUID::null();
    }
} // namespace TempoUI
