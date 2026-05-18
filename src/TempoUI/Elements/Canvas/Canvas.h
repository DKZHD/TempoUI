#pragma once
#include "Elements/ElementBase/ElementBase.h"

namespace TempoUI
{
    class CanvasConfig : public ElementConfig<CanvasConfig>
    {
    };

    class Canvas : public ElementBase
    {
    public:
        Canvas(const CanvasConfig& _config);
        Canvas(const std::string& id, glm::vec2 pos, glm::vec2 size);
        ~Canvas();

    private:
        void update_buffers() override;
    };
} // namespace TempoUI
