#pragma once
#include "AnimationManager/AnimationHandle.h"
#include "Elements/ElementBase/ElementBase.h"

namespace TempoUI
{
    enum class Direction : uint8_t
    {
        Vertical,
        Horizontal
    };

    class ScrollBoxConfig : public ElementConfig<ScrollBoxConfig>
    {
    public:
        ScrollBoxConfig& direction(Direction direction);
        ScrollBoxConfig& spacing(float spacing);
        [[nodiscard]] Direction direction() const { return direction_; }
        [[nodiscard]] float spacing() const { return spacing_; }

    private:
        float spacing_ = 0.f;
        Direction direction_ = Direction::Vertical;
    };

    class ScrollBox : public ElementBase
    {
    public:
        ScrollBox(const ScrollBoxConfig& _config);
        ScrollBox(const std::string& id, glm::vec2 pos, glm::vec2 size, float spacing, Direction direction = Direction::Vertical);
        ~ScrollBox();
        void reset_scroll();

    private:
        void self_update(float dt) override;
        void self_on_scroll(float yOffset) override;
        void update_buffers() override;

        Direction direction_ = Direction::Vertical;
        float local_offset_ = 0.f;
        float spacing_ = 0.f;
        float child_size_ = 0.f;

        AnimationHandle handle;
    };
} // namespace TempoUI
