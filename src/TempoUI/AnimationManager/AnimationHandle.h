#pragma once
#include "UUID/UUID.h"
#include "Animation/Animation.h"

namespace TempoUI
{
    class AnimationHandle
    {
    public:
        UUID uuid;
        [[nodiscard]] bool is_valid() const;
        [[nodiscard]] Animation* anim() const;
        AnimationHandle() = default;
        ~AnimationHandle();

        AnimationHandle(const AnimationHandle& other) = delete;
        AnimationHandle& operator=(const AnimationHandle& other) = delete;
        AnimationHandle& operator=(AnimationHandle&& other) noexcept;
        AnimationHandle(AnimationHandle&&) noexcept;
    };
} // namespace TempoUI
