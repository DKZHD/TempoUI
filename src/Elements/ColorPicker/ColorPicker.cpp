#include "ColorPicker.h"
#include "EventManager/EventManager.h"
#include "ThemeManager/ThemeManager.h"
#include "LayoutRegistry/LayoutRegistry.h"

namespace
{
    glm::vec3 barycentric_calculation(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 pos)
    {
        float determinant = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
        float l1 = ((p2.y - p3.y) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.y)) / determinant;
        float l2 = ((p3.y - p1.y) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.y)) / determinant;
        float l3 = 1 - l1 - l2;
        return { l1, l2, l3 };
    }
} // namespace
ColorPickerConfig::ColorPickerConfig()
{
    if (!ThemeManager::get().is_loaded())
        return;
    if (JSONToken* color_picker = ThemeManager::get().try_get("ColorPicker"))
    {
        for (const auto& [name, value] : color_picker->as_object())
        {
            if (name == "Selector Background Color")
            {
                selector_background_color_ = value.as_vec4();
            }
            if (name == "Selector Inner Diameter")
            {
                selector_inner_diameter_ = (float)value.as_number();
            }
            if (name == "Selector Outer Diameter")
            {
                selector_outer_diameter_ = (float)value.as_number();
            }
        }
    }
}
ColorPickerConfig& ColorPickerConfig::selector_background_color(glm::vec4 color)
{
    custom_values |= CustomFlags::ColorPickerValues::Selector_Background_Color;
    selector_background_color_ = color;
    return *this;
}
ColorPickerConfig& ColorPickerConfig::selector_outer_diameter(float diameter)
{
    custom_values |= CustomFlags::ColorPickerValues::Selector_Outer_Diameter;
    selector_outer_diameter_ = diameter;
    return *this;
}
ColorPickerConfig& ColorPickerConfig::selector_inner_diameter(float diameter)
{
    custom_values |= CustomFlags::ColorPickerValues::Selector_Inner_Diameter;
    selector_inner_diameter_ = diameter;
    return *this;
}

ColorPicker::ColorPicker(const ColorPickerConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex()), selector_inner_diameter_(_config.selector_inner_diameter()), selector_outer_diameter_(_config.selector_outer_diameter())
{
    config.state |= HOVERABLE;
    config.localPosition = _config.position();
    config.size = _config.size();
    selector_background_color_ = _config.selector_background_color();

    meshes_[0] = UIMesh(get_position(), config.size, glm::vec4(1.f), glm::vec4(0.f), -2.f);
    meshes_[1] = UIMesh(get_position() - (selector_outer_diameter_ * 0.5f), glm::vec2(selector_outer_diameter_), glm::vec4(glm::vec3(selector_background_color_), 0.f), glm::vec4(selector_outer_diameter_ * 0.5f));
    meshes_[2] = UIMesh(get_position() - (selector_outer_diameter_ * 0.5f) + ((selector_outer_diameter_ - selector_inner_diameter_) * 0.5f), glm::vec2(selector_inner_diameter_), glm::vec4(0.f, 0.f, 0.f, 0.f), glm::vec4(selector_inner_diameter_ * 0.5f));

    set_on_hover([this]()
    {
        meshes_[1].update_color(selector_background_color_);
        meshes_[2].update_color(glm::vec4(glm::vec3(get_color()), 1.f));
        mark_dirty();
    });
    set_on_unhover([this]()
    {
        if (!is_dragging)
        {
            meshes_[1].update_color(glm::vec4(glm::vec3(selector_background_color_), 0.f));
            meshes_[2].update_color(glm::vec4(glm::vec3(get_color()), 0.f));
            mark_dirty();
        }
    });
}

ColorPicker::ColorPicker(const std::string& id, glm::vec2 pos, glm::vec2 size) : ElementBase(id)
{
    config.state |= HOVERABLE;
    config.localPosition = pos;
    config.size = size;

    meshes_[0] = UIMesh(get_position(), size, glm::vec4(1.f), glm::vec4(0.f), -2.f);
    meshes_[1] = UIMesh(get_position() - (size.x * 0.06f), glm::vec2(size.x * 0.12f), glm::vec4(1.f, 1.f, 1.f, 0.f), glm::vec4(size.x * 0.06f));
    meshes_[2] = UIMesh(get_position() - (size.x * 0.05f), glm::vec2(size.x * 0.1f), glm::vec4(0.f, 0.f, 0.f, 0.f), glm::vec4(size.x * 0.05f));

    set_on_hover([this]()
    {
        meshes_[1].update_color(glm::vec4(glm::vec3(1.f), 1.f));
        meshes_[2].update_color(glm::vec4(glm::vec3(get_color()), 1.f));
        mark_dirty();
    });
    set_on_unhover([this]()
    {
        if (!is_dragging)
        {
            meshes_[1].update_color(glm::vec4(glm::vec3(1.f), 0.f));
            meshes_[2].update_color(glm::vec4(glm::vec3(get_color()), 0.f));
            mark_dirty();
        }
    });
}

glm::vec4 ColorPicker::get_color() const
{
    return meshes_[2].get_color();
}

void ColorPicker::update_buffers()
{
    vertices.clear();
    indices.clear();

    meshes_[0].update_size(config.size);
    meshes_[1].update_size(glm::vec2(selector_outer_diameter_ * global_scale_.y));
    meshes_[2].update_size(glm::vec2(selector_inner_diameter_ * global_scale_.y));

    meshes_[1].update_radius(glm::floor(glm::vec4(meshes_[1].get_size().x * 0.5f)));
    meshes_[2].update_radius(glm::floor(glm::vec4(meshes_[2].get_size().x * 0.5f)));

    glm::vec2 current_pos = get_position();
    meshes_[0].update_position(current_pos);

    glm::vec2 cursor_absolute_position = current_pos + (config.size * normalized_position_);
    meshes_[1].update_position(cursor_absolute_position - (meshes_[1].get_size() * 0.5f));
    meshes_[2].update_position(cursor_absolute_position - (meshes_[2].get_size() * 0.5f));

    std::array<Vertex, 4> gradient_vertices = meshes_[0].get_vertices();
    std::array<unsigned int, 6> gradient_indices = meshes_[0].get_indices();

    indices.insert(indices.end(), gradient_indices.begin(), gradient_indices.end());
    gradient_vertices[0].color = glm::vec4(0.f, 1.f, 0.f, 1.f);
    gradient_vertices[1].color = glm::vec4(0.f, 0.f, 1.f, 1.f);
    gradient_vertices[2].color = glm::vec4(1.f, 0.f, 0.f, 1.f);
    gradient_vertices[3].color = glm::vec4(1.f, 1.f, 1.f, 1.f);
    vertices.insert(vertices.end(), gradient_vertices.begin(), gradient_vertices.end());

    glm::vec4 top_color = glm::mix(glm::vec4(0.f, 1.f, 0.f, 1.f), glm::vec4(1.f), normalized_position_.x);
    glm::vec4 bottom_color = glm::mix(glm::vec4(0.f, 0.f, 1.f, 1.f), glm::vec4(1.f, 0.f, 0.f, 1.f), normalized_position_.x);

    glm::vec4 color = glm::mix(top_color, bottom_color, normalized_position_.y);

    meshes_[2].update_color(glm::vec4(glm::vec3(color), get_color().w));

    std::array<Vertex, 4> mesh_vertices2 = meshes_[1].get_vertices();
    std::array<unsigned int, 6> mesh_indices2 = meshes_[1].get_indices();

    for (int m_i = 0; m_i < 2; m_i++)
    {
        const std::array<Vertex, 4>& mesh_vertices = meshes_[m_i + 1].get_vertices();
        const std::array<unsigned int, 6>& mesh_indices = meshes_[m_i + 1].get_indices();

        for (int i = 0; i < 6; i++)
        {
            indices.emplace_back(vertices.size() + mesh_indices[i]);
        }
        vertices.insert(vertices.end(), mesh_vertices.begin(), mesh_vertices.end());
    }
}

void ColorPicker::self_on_mouse_button(UI::MouseButton button, UI::Action action)
{
    if (check_state(IS_HOVERED) && button == UI::MouseButton::Left && action == UI::Action::Press)
    {
        is_dragging = true;
    }
    else if (is_dragging && action == UI::Action::Release)
    {
        is_dragging = false;
        if (!check_state(IS_HOVERED))
        {
            meshes_[1].update_color(glm::vec4(glm::vec3(1.f), 0.f));
            meshes_[2].update_color(glm::vec4(glm::vec3(get_color()), 0.f));
            mark_dirty();
        }
    }
}

void ColorPicker::set_selector_position(glm::vec2 relative_pos)
{
    float circle_size = config.size.x * 0.1f;
    relative_pos += get_position();
    meshes_[1].update_position(relative_pos - ((circle_size * 1.2f) * 0.5f));
    meshes_[2].update_position(relative_pos - (circle_size * 0.5f));

    mark_dirty();
}
void ColorPicker::set_normalized_selector_position(glm::vec2 normalized_pos)
{
    float circle_size = config.size.x * 0.1f;
    normalized_pos *= config.size;
    normalized_pos += get_position();
    meshes_[1].update_position(normalized_pos - ((circle_size * 1.2f) * 0.5f));
    meshes_[2].update_position(normalized_pos - (circle_size * 0.5f));

    mark_dirty();
}

void ColorPicker::self_theme_updated()
{
    ColorPickerConfig cpc;
    if (!(custom_values & CustomFlags::ColorPickerValues::Selector_Background_Color))
    {
        selector_background_color_ = cpc.selector_background_color();
    }
    if (!(custom_values & CustomFlags::ColorPickerValues::Selector_Inner_Diameter))
    {
        selector_inner_diameter_ = cpc.selector_inner_diameter();
    }
    if (!(custom_values & CustomFlags::ColorPickerValues::Selector_Outer_Diameter))
    {
        selector_outer_diameter_ = cpc.selector_outer_diameter();
    }
}

void ColorPicker::self_update(float delta_time)
{
    if (is_dragging)
    {
        glm::dvec2 mouse_pos = EventManager::get().get_mouse_position();
        glm::vec2 final_pos = glm::clamp((glm::vec2)mouse_pos, get_position(), get_position() + config.size);
        normalized_position_ = glm::clamp((final_pos - get_position()) / config.size, glm::vec2(0.f), glm::vec2(1.f));
        mark_dirty();
    }
}

static LayoutRegistry::AutoRegister dummy("ColorPicker", [](const JSON_OBJECT& json_object, BaseProperties props) // NOLINT
{
    ColorPickerConfig config;
    config.id(props.Id.value());
    if (props.Pos)
        config.position(props.Pos.value());
    if (props.Size)
        config.size(props.Size.value());
    if (props.Flex)
        config.flex(props.Flex.value());

    if (auto it = json_object.find("Selector Inner Diameter"); it != json_object.end())
    {
        config.selector_inner_diameter((float)it->second.as_number());
    }
    if (auto it = json_object.find("Selector Outer Diameter"); it != json_object.end())
    {
        config.selector_outer_diameter((float)it->second.as_number());
    }
    if (auto it = json_object.find("Selector Background Color"); it != json_object.end())
    {
        config.selector_background_color(it->second.as_vec4());
    }
    return std::make_unique<ColorPicker>(config);
});