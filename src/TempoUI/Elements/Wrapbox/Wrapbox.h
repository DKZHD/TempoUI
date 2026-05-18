#pragma once
#include "Elements/ElementBase/ElementBase.h"

namespace TempoUI
{
    class WrapboxConfig : public ElementConfig<WrapboxConfig>
    {
    };

    class Wrapbox : public ElementBase
    {
    public:
        Wrapbox(const WrapboxConfig& _config);
        Wrapbox(const std::string& id, glm::vec2 pos, glm::vec2 size);
        ~Wrapbox();

    private:
        void self_update(float dt) override;
        void update_buffers() override;
    };
} // namespace TempoUI
