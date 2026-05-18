#include "HorizontalBox.h"
#include "LayoutRegistry/LayoutRegistry.h"

namespace TempoUI
{
    HorizontalBox::HorizontalBox(const HorizontalBoxConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex()), spacing_(_config.spacing())
    {
        config.size = _config.size();
        config.localPosition = _config.position();
        config.state |= HOVERABLE;
        config.state |= IS_CONTAINER;
    }

    HorizontalBox::HorizontalBox(const std::string& id, glm::vec2 pos, glm::vec2 size, float spacing) : ElementBase(id), spacing_(spacing)
    {
        config.size = size;
        config.localPosition = pos;
        config.state |= HOVERABLE;
        config.state |= IS_CONTAINER;
    }

    HorizontalBox::~HorizontalBox()
    {
    }

    void HorizontalBox::self_update(float dt)
    {
    }

    void HorizontalBox::update_buffers()
    {
        if (children.empty())
            return;

        float x_offset = 0.f;
        float occupied_space = 0.f;
        int flex_count = 0;

        occupied_space += ((float)children.size() - 1.f) * spacing_;

        for (const auto& child : children)
        {
            if (int i = child->get_flex(); i == 0)
            {
                occupied_space += child->get_size().x;
            }
            else
            {
                flex_count += i;
            }
        }

        float remaining_space = std::max(0.f, config.size.x - occupied_space);
        float length_segments = flex_count > 0 ? remaining_space / (float)flex_count : 0.f;

        for (auto& child : children)
        {
            glm::vec2 child_size = child->get_size();
            if (int flex = child->get_flex(); flex > 0)
            {
                child_size.x = length_segments * (float)flex;
                child->set_size(child_size);
            }

            glm::vec2 child_pos = child->get_local_position();
            child->set_local_position({ x_offset, child_pos.y });

            x_offset += child_size.x + spacing_;

            child->update_absolute_position();
            child->update_buffers();
        }
    }

    HorizontalBoxConfig& HorizontalBoxConfig::spacing(float spacing)
    {
        spacing_ = spacing;
        return *this;
    }

    static LayoutRegistry::AutoRegister dummy("HorizontalBox", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
    {
        HorizontalBoxConfig config;
        config.id(props.Id.value());
        if (props.Pos)
            config.position(props.Pos.value());
        if (props.Size)
            config.size(props.Size.value());
        if (props.Radius)
            config.radius(props.Radius.value());
        if (props.Flex)
            config.flex(props.Flex.value());

        if (auto it = json_object.find("Spacing"); it != json_object.end())
        {
            config.spacing((float)it->second.as_number());
        }
        return std::make_unique<HorizontalBox>(config);
    });
} // namespace TempoUI
