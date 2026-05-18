#pragma once
#include "Elements/ElementBase/ElementBase.h"

namespace TempoUI
{
    class VerticalBoxConfig : public ElementConfig<VerticalBoxConfig>
    {
    public:
        VerticalBoxConfig& spacing(float spacing);
        [[nodiscard]] float spacing() const { return spacing_; }

    private:
        float spacing_ = 0.f;
    };

    class VerticalBox : public ElementBase
    {
    public:
        VerticalBox(const VerticalBoxConfig& _config);
        VerticalBox(const std::string& id, glm::vec2 pos, glm::vec2 size, float spacing);
        ~VerticalBox();

    private:
        void self_update(float dt) override;
        void update_buffers() override;

        float spacing_ = 0.f;
    };
} // namespace TempoUI
