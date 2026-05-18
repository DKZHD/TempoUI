#include "Wrapbox.h"
#include "LayoutRegistry/LayoutRegistry.h"

namespace TempoUI
{
    Wrapbox::Wrapbox(const WrapboxConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex())
    {
        config.size = _config.size();
        config.localPosition = _config.position();
        config.state |= HOVERABLE;
    }

    Wrapbox::Wrapbox(const std::string& id, glm::vec2 pos, glm::vec2 size) : ElementBase(id)
    {
        config.size = size;
        config.localPosition = pos;
        config.state |= HOVERABLE;
    }

    Wrapbox::~Wrapbox()
    {
    }

    void Wrapbox::self_update(float dt)
    {
    }

    void Wrapbox::update_buffers()
    {
        float x_size = 0.f;
        float current_y = 0.f;
        float max_y_size = 0.f;

        for (auto& child : children)
        {
            float current = x_size + child->get_size().x;
            if (current > config.size.x)
            {
                x_size = 0.f;
                current_y += max_y_size;
                max_y_size = 0.f;
            }
            glm::vec2 pos = child->get_local_position();
            pos.x = x_size;
            pos.y = current_y;
            child->set_local_position(pos);

            x_size += child->get_size().x;
            max_y_size = std::max(max_y_size, child->get_size().y);

            child->update_absolute_position();
            child->update_buffers();
        }
    }

    static LayoutRegistry::AutoRegister dummy("Wrapbox", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
    {
        WrapboxConfig config;
        config.id(props.Id.value());
        if (props.Pos)
            config.position(props.Pos.value());
        if (props.Size)
            config.size(props.Size.value());
        if (props.Flex)
            config.flex(props.Flex.value());

        return std::make_unique<Wrapbox>(config);
    });
} // namespace TempoUI
