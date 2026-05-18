#pragma once
#include "Elements/ElementBase/ElementBase.h"

namespace TempoUI
{
    class HorizontalBoxConfig : public ElementConfig<HorizontalBoxConfig>
    {
    public:
        HorizontalBoxConfig& spacing(float spacing);
        [[nodiscard]] float spacing() const { return spacing_; };

    private:
        float spacing_ = 0.f;
    };

    class HorizontalBox : public ElementBase
    {
    public:
        HorizontalBox(const HorizontalBoxConfig& _config);
        HorizontalBox(const std::string& id, glm::vec2 pos, glm::vec2 size, float spacing);
        ~HorizontalBox();

    private:
        void self_update(float dt) override;
        void update_buffers() override;

        float spacing_ = 0.f;
    };
} // namespace TempoUI
