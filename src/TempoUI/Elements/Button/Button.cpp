#include "Button.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "UIMesh/UIMesh.h"
#include "AnimationManager/AnimationManager.h"
#include "ThemeManager/ThemeManager.h"

namespace TempoUI
{
    Button::Button(const ButtonConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex())
    {
        config.localPosition = _config.position();
        config.size = _config.size();
        config.radius = _config.radius();

        default_color_ = _config.color();
        hover_color_ = _config.hover_color();
        clicked_color_ = _config.clicked_color();

        config.state |= HOVERABLE;
        config.state |= CLICKABLE;

        button_mesh = std::make_unique<UIMesh>(get_position(), _config.size(), default_color_, _config.radius());
    }

    Button::Button(const std::string& id, glm::vec2 pos, glm::vec2 size,
        glm::vec4 color, glm::vec4 radius)
        : ElementBase(id), default_color_(color)
    {
        config.localPosition = pos;
        config.size = size;
        config.radius = radius;

        hover_color_ = color * 1.2f;
        clicked_color_ = glm::vec4(1.f - glm::vec3(color), 0.7f);

        config.state |= HOVERABLE;
        config.state |= CLICKABLE;

        button_mesh = std::make_unique<UIMesh>(pos, size, color, radius);
    }

    void Button::set_on_click(std::function<void()> func)
    {
        on_click = std::move(func);
    }

    void Button::trigger_on_click()
    {
        if (on_click)
        {
            on_click();
        }
    }

    void Button::self_on_mouse_button(UI::MouseButton button, UI::Action action)
    {
        if (((config.state & IS_HOVERED) != 0) && button == UI::MouseButton::Left &&
            action == UI::Action::Press)
        {
            if (on_click)
                on_click();
            glm::vec4 color = button_mesh->get_color();
            handle = Animate::Start([this, color](float x)
            {
                glm::vec4 stored_color = color;
                stored_color.x = std::lerp(color.x, clicked_color_.x, x);
                stored_color.y = std::lerp(color.y, clicked_color_.y, x);
                stored_color.z = std::lerp(color.z, clicked_color_.z, x);
                stored_color.w = std::lerp(color.w, clicked_color_.w, x);
                button_mesh->update_color(stored_color);
                mark_dirty();
            }, 0.1f, AnimationType::Perlin_SmoothStep);
        }
        else if (((config.state & IS_HOVERED) != 0) && button == UI::MouseButton::Left &&
                 action == UI::Action::Release)
        {
            glm::vec4 color = button_mesh->get_color();
            handle = Animate::Start([this, color](float x)
            {
                glm::vec4 stored_color = color;
                stored_color.x = std::lerp(color.x, hover_color_.x, x);
                stored_color.y = std::lerp(color.y, hover_color_.y, x);
                stored_color.z = std::lerp(color.z, hover_color_.z, x);
                stored_color.w = std::lerp(color.w, hover_color_.w, x);
                button_mesh->update_color(stored_color);
                mark_dirty();
            }, 0.1f, AnimationType::Perlin_SmoothStep);
        }
    }

    void Button::self_update(float dt) {}

    void Button::on_hover()
    {
        glm::vec4 color = button_mesh->get_color();
        handle = Animate::Start([this, color](float x)
        {
            glm::vec4 stored_color = color;
            stored_color.x = std::lerp(color.x, hover_color_.x, x);
            stored_color.y = std::lerp(color.y, hover_color_.y, x);
            stored_color.z = std::lerp(color.z, hover_color_.z, x);
            stored_color.w = std::lerp(color.w, hover_color_.w, x);

            button_mesh->update_color(stored_color);
            mark_dirty();
        }, 0.1f, AnimationType::Perlin_SmoothStep);
        ElementBase::on_hover();
        mark_dirty();
    }

    void Button::on_unhover()
    {
        glm::vec4 color = button_mesh->get_color();
        handle = Animate::Start([this, color](float x)
        {
            glm::vec4 stored_color = color;
            stored_color.x = std::lerp(color.x, default_color_.x, x);
            stored_color.y = std::lerp(color.y, default_color_.y, x);
            stored_color.z = std::lerp(color.z, default_color_.z, x);
            stored_color.w = std::lerp(color.w, default_color_.w, x);

            button_mesh->update_color(stored_color);
            mark_dirty();
        }, 0.1f, AnimationType::Perlin_SmoothStep);
        ElementBase::on_unhover();
        mark_dirty();
    }

    void Button::update_buffers()
    {
        vertices.clear();
        indices.clear();

        if (parent != nullptr)
        {
            button_mesh->update_position(get_position());
            button_mesh->update_size(config.size);
        }

        vertices.reserve(4);
        indices.reserve(6);

        for (unsigned int index : button_mesh->get_indices())
        {
            indices.push_back(index + vertices.size());
        }
        vertices.insert(vertices.end(), button_mesh->get_vertices().begin(),
            button_mesh->get_vertices().end());

        for (const auto& child : children)
        {
            child->update_absolute_position();
            child->update_buffers();
        }
    }

    void Button::self_theme_updated()
    {
        ButtonConfig _config = ButtonConfig().id(ID).position(config.localPosition).size(config.size); // NOLINT

        if (!(custom_values & CustomFlags::ButtonValues::Default_Color))
        {
            default_color_ = _config.color();
        }
        if (!(custom_values & CustomFlags::ButtonValues::Hover_Color))
        {
            hover_color_ = _config.hover_color();
        }
        if (!(custom_values & CustomFlags::ButtonValues::Clicked_Color))
        {
            clicked_color_ = _config.clicked_color();
        }
        if (!(custom_values & CustomFlags::Radius))
        {
            config.radius = _config.radius();
            button_mesh->update_radius(config.radius);
        }
        button_mesh->update_color(default_color_);
    }

    void Button::update_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::ButtonValues::Default_Color;
        custom_values |= CustomFlags::ButtonValues::Hover_Color;
        custom_values |= CustomFlags::ButtonValues::Clicked_Color;

        default_color_ = color;
        hover_color_ = color * 1.2f;
        clicked_color_ = glm::vec4(1.f - glm::vec3(color), 0.7f);
        button_mesh->update_color(color);
    }
    void Button::update_radius(glm::vec4 radius)
    {
        button_mesh->update_radius(radius);
    }

    ButtonConfig::ButtonConfig()
    {
        if (!ThemeManager::get().is_loaded())
        {
            return;
        }
        if (JSONToken* button = ThemeManager::get().try_get("Button"))
        {
            const std::map<std::string, JSONToken>& mapped_values = button->as_object();
            for (const auto& [name, value] : mapped_values)
            {
                if (name == "Default Color")
                {
                    default_color_ = value.as_vec4();
                }
                else if (name == "Hover Color")
                {
                    hover_color_ = value.as_vec4();
                }
                else if (name == "Click Color")
                {
                    clicked_color_ = value.as_vec4();
                }
                else if (name == "Radius")
                {
                    radius_ = (value.as_vec4());
                }
            }
        }
    }

    static LayoutRegistry::AutoRegister dummy("Button", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
    {
        ButtonConfig config;
        config.id(props.Id.value());
        if (props.Pos)
            config.position(props.Pos.value());
        if (props.Size)
            config.size(props.Size.value());
        if (props.Radius)
            config.radius(props.Radius.value());
        if (props.Flex)
            config.flex(props.Flex.value());

        if (auto it = json_object.find("Default Color"); it != json_object.end())
        {
            config.color(it->second.as_vec4());
        }
        if (auto it = json_object.find("Clicked Color"); it != json_object.end())
        {
            config.clicked_color(it->second.as_vec4());
        }
        if (auto it = json_object.find("Hover Color"); it != json_object.end())
        {
            config.hover_color(it->second.as_vec4());
        }

        return std::make_unique<Button>(config);
    });
} // namespace TempoUI
