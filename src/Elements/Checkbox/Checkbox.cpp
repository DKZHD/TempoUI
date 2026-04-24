#include "Checkbox.h"
#include "AnimationManager/AnimationManager.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "ThemeManager/ThemeManager.h"

Checkbox::Checkbox(const CheckboxConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex()), checked_(_config.default_state())
{
    config.state |= HOVERABLE;
    config.state |= CLICKABLE;
    config.localPosition = _config.position();
    config.size = _config.size();
    config.radius = _config.radius();

    check_color_ = _config.check_color();
    background_color_ = _config.background_color();
    hover_color_ = _config.hover_color();

    meshes_[0] = UIMesh(get_position(), _config.size(), background_color_, _config.radius());
    glm::vec2 offset = _config.size() * 0.1f;

    meshes_[1] = UIMesh(offset + get_position(), _config.size() - (offset * 2.f), glm::vec4(0.f, 0.f, 0.f, 0.f), glm::max(config.radius - (config.size.x * 0.1f), glm::vec4(0.f)));

    set_on_hover([this]()
    {
        meshes_[1].update_color(hover_color_);
        mark_dirty();
    });
    set_on_unhover([this]()
    {
        glm::vec4 color = checked_ ? check_color_ : glm::vec4(0.f, 0.f, 0.f, 0.f);
        meshes_[1].update_color(color);
        mark_dirty();
    });
}

Checkbox::Checkbox(const std::string& id, glm::vec2 pos, glm::vec2 size, glm::vec4 radius, bool default_state) : ElementBase(id), checked_(default_state)
{
    config.state |= HOVERABLE;
    config.state |= CLICKABLE;
    config.localPosition = pos;
    config.size = size;

    meshes_[0] = UIMesh(get_position(), size, glm::vec4(1.f), radius);
    glm::vec2 offset = size * 0.1f;

    meshes_[1] = UIMesh(offset + get_position(), size - (offset * 2.f), glm::vec4(0.f, 0.f, 0.f, 0.f), glm::max(config.radius - (config.size.x * 0.1f), glm::vec4(0.f)));

    set_on_hover([this]()
    {
        meshes_[1].update_color(hover_color_);
        mark_dirty();
    });
    set_on_unhover([this]()
    {
        glm::vec4 color = checked_ ? check_color_ : glm::vec4(0.f, 0.f, 0.f, 0.f);
        meshes_[1].update_color(color);
        mark_dirty();
    });
}

void Checkbox::bind_variable(bool& value)
{
    attached_variable_ = &value;
    value = checked_;
}

bool Checkbox::get_value() const
{
    return checked_;
}

void Checkbox::set_value(bool new_value)
{
    if (attached_variable_ != nullptr)
    {
        *attached_variable_ = new_value;
    }
    checked_ = new_value;
}

void Checkbox::self_update(float delta_time)
{
}

void Checkbox::self_on_mouse_button(UI::MouseButton button, UI::Action action)
{
    if (!(config.state & IS_HOVERED) || action != UI::Action::Press)
        return;

    switch (button)
    {
    case UI::MouseButton::Left:
    {
        checked_ = !checked_;
        if (attached_variable_ != nullptr)
        {
            *attached_variable_ = checked_;
        }

        if (checked_)
        {
            handle = Animate::Start([this](float x)
            {
                glm::vec4 color = meshes_[1].get_color();
                color.w = std::lerp(0.f, 1.f, x);
                meshes_[1].update_color(color);
                mark_dirty();
            }, 0.2f, AnimationType::Perlin_SmoothStep);
        }
        else
        {
            handle = Animate::Start([this](float x)
            {
                glm::vec4 color = meshes_[1].get_color();
                color.w = std::lerp(1.f, 0.f, x);
                meshes_[1].update_color(color);
                mark_dirty();
            }, 0.2f, AnimationType::Perlin_SmoothStep);
        }
    }
    break;
    default:
        break;
    }
}

void Checkbox::self_theme_updated()
{
    CheckboxConfig _config = CheckboxConfig().id(ID).position(config.localPosition); // NOLINT
    if (!(custom_values & CustomFlags::CheckboxValues::Check_Color))
    {
        check_color_ = _config.check_color();
    }
    if (!(custom_values & CustomFlags::CheckboxValues::Hover_Color))
    {
        hover_color_ = _config.hover_color();
    }
    if (!(custom_values & CustomFlags::CheckboxValues::Background_Color))
    {
        background_color_ = _config.background_color();
    }
    if (!(custom_values & CustomFlags::Radius))
    {
        config.radius = _config.radius();
        meshes_[0].update_radius(config.radius);
        meshes_[1].update_radius(glm::max(config.radius - (config.size.x * 0.1f), glm::vec4(0.f)));
    }
    if (checked_)
    {
        meshes_[1].update_color(check_color_);
    }
}

void Checkbox::update_buffers()
{
    vertices.clear();
    indices.clear();

    vertices.reserve(8);
    indices.reserve(12);

    if (parent)
    {
        glm::vec2 offset = config.size * 0.1f;

        meshes_[0].update_position(get_position());
        meshes_[1].update_position(offset + get_position());

        meshes_[0].update_size(config.size);
        meshes_[1].update_size(config.size - (offset * 2.f));
    }

    for (UIMesh& mesh : meshes_)
    {
        for (const unsigned int& index : mesh.get_indices())
        {
            indices.emplace_back(vertices.size() + index);
        }
        const std::array<Vertex, 4> mesh_vertices = mesh.get_vertices();
        vertices.insert(vertices.end(), mesh_vertices.begin(), mesh_vertices.end());
    }

    for (const auto& child : children)
    {
        child->update_absolute_position();
        child->update_buffers();
    }
}

CheckboxConfig& CheckboxConfig::default_state(bool _default)
{
    default_ = _default;
    return *this;
}

CheckboxConfig& CheckboxConfig::check_color(glm::vec4 color)
{
    custom_values |= CustomFlags::CheckboxValues::Check_Color;
    check_color_ = color;
    return *this;
}

CheckboxConfig& CheckboxConfig::background_color(glm::vec4 color)
{
    custom_values |= CustomFlags::CheckboxValues::Background_Color;
    background_color_ = color;
    return *this;
}

CheckboxConfig& CheckboxConfig::hover_color(glm::vec4 color)
{
    custom_values |= CustomFlags::CheckboxValues::Hover_Color;
    hover_color_ = color;
    return *this;
}

CheckboxConfig::CheckboxConfig()
{
    if (!ThemeManager::get().is_loaded())
    {
        return;
    }
    if (JSONToken* checkbox = ThemeManager::get().try_get("Checkbox"))
    {
        const std::map<std::string, JSONToken>& mapped_values = checkbox->as_object();
        for (const auto& [name, value] : mapped_values)
        {
            if (name == "Check Color")
            {
                check_color_ = value.as_vec4();
            }
            else if (name == "Hover Color")
            {
                hover_color_ = value.as_vec4();
            }
            else if (name == "Background Color")
            {
                background_color_ = value.as_vec4();
            }
            else if (name == "Radius")
            {
                radius_ = (value.as_vec4());
            }
        }
    }
}

static LayoutRegistry::AutoRegister dummy("Checkbox", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
{
    CheckboxConfig config;
    config.id(props.Id.value());
    if (props.Pos)
        config.position(props.Pos.value());
    if (props.Size)
        config.size(props.Size.value());
    if (props.Radius)
        config.radius(props.Radius.value());
    if (props.Flex)
        config.flex(props.Flex.value());

    if (auto it = json_object.find("Check Color"); it != json_object.end())
    {
        config.check_color(it->second.as_vec4());
    }
    if (auto it = json_object.find("Background Color"); it != json_object.end())
    {
        config.background_color(it->second.as_vec4());
    }
    if (auto it = json_object.find("Hover Color"); it != json_object.end())
    {
        config.hover_color(it->second.as_vec4());
    }
    return std::make_unique<Checkbox>(config);
});
