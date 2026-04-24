#include "Slider.h"
#include "EventManager/EventManager.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "ThemeManager/ThemeManager.h"

Slider::Slider(const SliderConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex()), slider_size(_config.slider_size()), min_(_config.min()), max_(_config.max()), color_(_config.color())
{
    config.size = _config.size();
    config.localPosition = _config.position();
    config.radius = _config.radius();
    config.state |= HOVERABLE;
    config.state |= CLICKABLE;

    meshes_[0] = UIMesh(_config.position(), _config.size(), _config.color(), _config.radius());
    meshes_[1] = UIMesh(_config.position() + glm::vec2((_config.size().x * 0.5f) - (slider_size * 0.5f), 0.f), glm::vec2(slider_size, _config.size().y), glm::vec4(glm::vec3(1.f - _config.color()), 0.2f));
}

Slider::Slider(const std::string& id, glm::vec2 pos, glm::vec2 size, glm::vec4 color, glm::vec4 radius, float min, float max) : ElementBase(id), min_(min), max_(max), color_(color)
{
    config.size = size;
    config.localPosition = pos;
    config.radius = radius;
    config.state |= HOVERABLE;
    config.state |= CLICKABLE;

    meshes_[0] = UIMesh(pos, size, color, radius);
    meshes_[1] = UIMesh(pos + glm::vec2((size.x * 0.5f) - (slider_size * 0.5f), 0.f), glm::vec2(slider_size, size.y), glm::vec4(glm::vec3(1.f - color), 0.2f));
}

Slider::~Slider()
{
}

void Slider::set_value(float value)
{
    value_ = std::clamp(value, min_, max_);
    if (attached_variable_ != nullptr)
    {
        *attached_variable_ = value_ * (max_ - min_);
    }
    mark_dirty();
}

float Slider::get_value() const
{
    return value_ * (max_ - min_);
}

void Slider::bind_variable(float& variable)
{
    attached_variable_ = &variable;
    set_value(*attached_variable_ / max_);
}

void Slider::self_on_scroll(float yOffset)
{
    if (!check_state(IS_HOVERED))
    {
        return;
    }

    set_value(value_ + (yOffset * 0.05f));
}

void Slider::self_on_mouse_button(UI::MouseButton button, UI::Action action)
{
    if (isHovered() && action == UI::Action::Press && button == UI::MouseButton::Left)
    {
        is_dragging_ = true;
    }
    else if (action == UI::Action::Release && button == UI::MouseButton::Left)
    {
        is_dragging_ = false;
    }
}

void Slider::self_on_char(char character)
{
}

void Slider::self_update(float dt)
{
    const bool hovered = isHovered();
    if (is_dragging_ || (!(config.state & IS_HOVERED) && hovered))
    {
        config.state |= IS_HOVERED;
        meshes_[1].update_color(glm::vec4(glm::vec3(1.f - color_), 1.f));
        mark_dirty();
    }
    else if (config.state & IS_HOVERED && !hovered)
    {
        config.state &= ~IS_HOVERED;
        meshes_[1].update_color(glm::vec4(glm::vec3(1.f - color_), 0.2f));
        mark_dirty();
    }
    if (is_dragging_)
    {
        const glm::vec2 mouse_pos = EventManager::get().get_mouse_position();
        const glm::vec2 relative_pos = glm::clamp(mouse_pos - get_position(), glm::vec2(0.f), config.size);
        const float value = glm::clamp(relative_pos.x / config.size.x, min_, max_);
        set_value(value);
    }
}

void Slider::update_buffers()
{
    vertices.clear();
    indices.clear();

    vertices.reserve(meshes_.size() * 4);
    indices.reserve(meshes_.size() * 6);

    meshes_[0].update_size(config.size);
    meshes_[1].update_size({ slider_size * global_scale_.x, config.size.y });

    if (parent != nullptr)
    {
        meshes_[0].update_position(get_position());
        const glm::vec2 element_pos = get_position();
        const glm::vec2 slider_pos = glm::clamp(element_pos + glm::vec2(config.size.x * value_ - slider_size * 0.5f, 0.f), element_pos, element_pos + glm::vec2(config.size.x - slider_size, 0.f));
        meshes_[1].update_position(slider_pos);
    }

    for (UIMesh& mesh : meshes_)
    {
        const std::array<unsigned int, 6>& mesh_indices = mesh.get_indices();
        for (unsigned int index : mesh_indices)
        {
            indices.emplace_back(index + vertices.size());
        }

        std::array<Vertex, 4> mesh_vertices = mesh.get_vertices();

        vertices.insert(vertices.end(), mesh_vertices.begin(), mesh_vertices.end());
    }
}

void Slider::self_theme_updated()
{
    SliderConfig slider_config;
    if (!(custom_values & CustomFlags::SliderValues::Min))
    {
        min_ = slider_config.min();
    }
    if (!(custom_values & CustomFlags::SliderValues::Max))
    {
        max_ = slider_config.max();
    }
    if (!(custom_values & CustomFlags::SliderValues::Color))
    {
        color_ = slider_config.color();
        meshes_[0].update_color(color_);
        meshes_[1].update_color(glm::vec4(glm::vec3(1.f - color_), 0.2f));
    }
    if (!(custom_values & CustomFlags::Radius))
    {
        config.radius = slider_config.radius();
        meshes_[0].update_radius(config.radius);
    }
}

SliderConfig::SliderConfig()
{
    if (!ThemeManager::get().is_loaded())
        return;
    if (JSONToken* slider = ThemeManager::get().try_get("Slider"))
    {
        std::map<std::string, JSONToken> mapped_values = slider->as_object();
        for (const auto& [name, value] : mapped_values)
        {
            if (name == "Min")
            {
                min_ = (float)value.as_number();
            }
            else if (name == "Max")
            {
                max_ = (float)value.as_number();
            }
            else if (name == "Color")
            {
                color_ = value.as_vec4();
            }
            else if (name == "Radius")
            {
                radius_ = value.as_vec4();
            }
        }
    }
}

SliderConfig& SliderConfig::min(float min)
{
    custom_values |= CustomFlags::SliderValues::Min;
    min_ = min;
    return *this;
}

SliderConfig& SliderConfig::max(float max)
{
    custom_values |= CustomFlags::SliderValues::Max;
    max_ = max;
    return *this;
}

SliderConfig& SliderConfig::slider_size(float size)
{
    custom_values |= CustomFlags::SliderValues::Slider_Size;
    slider_size_ = size;
    return *this;
}

SliderConfig& SliderConfig::color(glm::vec4 color)
{
    custom_values |= CustomFlags::SliderValues::Color;
    color_ = color;
    return *this;
}

static LayoutRegistry::AutoRegister dummy("Slider", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
{
    SliderConfig config;
    config.id(props.Id.value());
    if (props.Pos)
        config.position(props.Pos.value());
    if (props.Size)
        config.size(props.Size.value());
    if (props.Radius)
        config.radius(props.Radius.value());
    if (props.Flex)
        config.flex(props.Flex.value());

    if (auto it = json_object.find("Color"); it != json_object.end())
    {
        config.color(it->second.as_vec4());
    }
    if (auto it = json_object.find("Min"); it != json_object.end())
    {
        config.min((float)it->second.as_number());
    }
    if (auto it = json_object.find("Max"); it != json_object.end())
    {
        config.max((float)it->second.as_number());
    }
    if (auto it = json_object.find("Slider Size"); it != json_object.end())
    {
        config.slider_size((float)it->second.as_number());
    }
    return std::make_unique<Slider>(config);
});
