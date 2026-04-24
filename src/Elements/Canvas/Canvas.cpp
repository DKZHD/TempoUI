#include "Canvas.h"
#include "LayoutRegistry/LayoutRegistry.h"

Canvas::Canvas(const std::string& id, glm::vec2 pos, glm::vec2 size)
    : ElementBase(id)
{
    config.localPosition = pos;
    config.size = size;
    config.state |= HOVERABLE;
    config.state |= IS_CONTAINER;
    config.state |= CLIP_CHILDREN;
}

Canvas::~Canvas() {}

Canvas::Canvas(const CanvasConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex())
{
    config.localPosition = _config.position();
    config.size = _config.size();
    config.state |= HOVERABLE;
    config.state |= IS_CONTAINER;
}

void Canvas::update_buffers()
{
    for (const auto& child : children)
    {
        child->update_absolute_position();
        child->update_buffers();
    }
}

static LayoutRegistry::AutoRegister dummy("Canvas", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
{
    CanvasConfig config;
    config.id(props.Id.value());
    if (props.Pos)
        config.position(props.Pos.value());
    if (props.Size)
        config.size(props.Size.value());
    if (props.Flex)
        config.flex(props.Flex.value());

    return std::make_unique<Canvas>(config);
});
