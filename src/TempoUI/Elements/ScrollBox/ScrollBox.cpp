#include "ScrollBox.h"
#include "AnimationManager/AnimationManager.h"
#include "LayoutRegistry/LayoutRegistry.h"

namespace TempoUI
{
    ScrollBox::ScrollBox(const ScrollBoxConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex()), spacing_(_config.spacing()), direction_(_config.direction())
    {
        config.size = _config.size();
        config.localPosition = _config.position();
        config.state |= HOVERABLE;
        config.state |= CLIP_CHILDREN;
    }

    ScrollBox::ScrollBox(const std::string& id, glm::vec2 pos, glm::vec2 size, float spacing, Direction direction) : ElementBase(id), spacing_(spacing), direction_(direction)
    {
        config.size = size;
        config.localPosition = pos;
        config.state |= HOVERABLE;
        config.state |= CLIP_CHILDREN;
    }

    ScrollBox::~ScrollBox()
    {
    }

    void ScrollBox::reset_scroll()
    {
    }

    void ScrollBox::self_update(float dt)
    {
    }

    void ScrollBox::self_on_scroll(float yOffset)
    {
        if (!isHovered())
            return;
        float size_dir = 0.f;
        if (direction_ == Direction::Vertical)
            size_dir = config.size.y;
        else
            size_dir = config.size.x;

        float start_offset = local_offset_;
        handle = Animate::Start([this, start_offset, yOffset, size_dir](float x)
        {
            local_offset_ = std::lerp(start_offset, start_offset + (yOffset * 50.f), x);
            local_offset_ = std::clamp(local_offset_, size_dir - child_size_, 0.f);
            mark_dirty();
        }, 0.03f, AnimationType::Perlin_SmoothStep);
    }

    void ScrollBox::update_buffers()
    {
        child_size_ = 0.f;
        float offset = 0.f;

        glm::vec2 pos = get_position();
        glm::vec2 size = get_size();

        for (const auto& child : children)
        {
            glm::vec2 child_pos = child->get_local_position();
            float size_dir = direction_ == Direction::Vertical ? child->get_size().y : child->get_size().x;
            if (direction_ == Direction::Vertical)
            {
                child_pos.y = local_offset_ + offset;
            }
            else
            {
                child_pos.x = local_offset_ + offset;
            }

            child->set_local_position(child_pos);
            offset += spacing_;
            offset += size_dir;
            child_size_ += size_dir + spacing_;
            child->update_absolute_position();

            glm::vec2 abs_child_pos = child->get_position();

            if (abs_child_pos.x + child->get_size().x >= pos.x && abs_child_pos.x <= pos.x + size.x &&
                abs_child_pos.y + child->get_size().y >= pos.y && abs_child_pos.y <= pos.y + size.y)
            {
                child->update_buffers();
                for (const auto& child_child : child->get_children())
                {
                    child_child->update_absolute_position();
                    child_child->update_buffers();
                }
            }
        }
    }

    ScrollBoxConfig& ScrollBoxConfig::direction(Direction direction)
    {
        direction_ = direction;
        return *this;
    }

    ScrollBoxConfig& ScrollBoxConfig::spacing(float spacing)
    {
        spacing_ = spacing;
        return *this;
    }

    static LayoutRegistry::AutoRegister dummy("ScrollBox", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
    {
        ScrollBoxConfig config;
        config.id(props.Id.value());
        if (props.Pos)
            config.position(props.Pos.value());
        if (props.Size)
            config.size(props.Size.value());
        if (props.Radius)
            config.radius(props.Radius.value());
        if (props.Flex)
            config.flex(props.Flex.value());

        if (auto it = json_object.find("Vertical"); it != json_object.end())
        {
            config.direction(it->second.as_bool() ? Direction::Vertical : Direction::Horizontal);
        }
        if (auto it = json_object.find("Spacing"); it != json_object.end())
        {
            config.spacing((float)it->second.as_number());
        }
        return std::make_unique<ScrollBox>(config);
    });

} // namespace TempoUI
