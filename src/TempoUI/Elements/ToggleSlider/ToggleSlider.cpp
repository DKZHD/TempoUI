#include "ToggleSlider.h"
#include "Elements/Button/Button.h"
#include "Elements/BorderedBox/BorderedBox.h"
#include "Animation/Animation.h"
#include "AnimationManager/AnimationManager.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "ThemeManager/ThemeManager.h"

namespace TempoUI
{
    ToggleSlider::ToggleSlider(const ToggleSliderConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex())
    {
        config.localPosition = _config.position();
        config.size = _config.size();
        config.state |= HOVERABLE;
        config.state |= CLICKABLE;

        slider_color_ = _config.slider_color();
        hover_color_ = _config.hover_color();
        background_color_ = _config.background_color();

        float inset = (_config.size().y * 0.5f) - 4.f;
        meshes[0] = UIMesh(get_position(), _config.size(), glm::vec4(background_color_), glm::vec4(inset + 4.f));
        meshes[1] = UIMesh(get_position() + glm::vec2(4.f), glm::vec2(inset * 2.f), slider_color_, glm::vec4(inset));

        set_on_hover([this]()
        {
            meshes[1].update_color(hover_color_);
            mark_dirty();
        });
        set_on_unhover([this]()
        {
            meshes[1].update_color(slider_color_);
            mark_dirty();
        });
    }

    ToggleSlider::ToggleSlider(const std::string& id, glm::vec2 pos, glm::vec2 size, glm::vec4 color) : ElementBase(id)
    {
        config.localPosition = pos;
        config.size = size;
        config.state |= HOVERABLE;
        config.state |= CLICKABLE;

        float inset = (size.y * 0.5f) - 4.f;
        meshes[0] = UIMesh(get_position(), size, glm::vec4(1.f), glm::vec4(inset + 4.f));
        meshes[1] = UIMesh(get_position() + glm::vec2(4.f), glm::vec2(inset * 2.f), color, glm::vec4(inset));

        set_on_hover([this]()
        {
            meshes[1].update_color(glm::vec4(0.2, 0.2, 0.2, 1.f));
            mark_dirty();
        });
        set_on_unhover([this, color]()
        {
            meshes[1].update_color(glm::vec4(color));
            mark_dirty();
        });
    }

    void ToggleSlider::bind_variable(bool& var)
    {
        bound_variable = &var;
        toggled = var;
    }

    void ToggleSlider::update_buffers()
    {
        vertices.clear();
        indices.clear();

        vertices.reserve(8);
        indices.reserve(12);

        meshes[0].update_position(get_position());
        meshes[1].update_position(glm::vec2(meshes[1].get_position().x, get_position().y + 4.f));

        for (UIMesh& mesh : meshes)
        {
            for (const unsigned int& index : mesh.get_indices())
            {
                indices.emplace_back(vertices.size() + index);
            }
            const std::array<Vertex, 4> mesh_vertices = mesh.get_vertices();
            vertices.insert(vertices.end(), mesh_vertices.begin(), mesh_vertices.end());
        }
    }

    void ToggleSlider::self_theme_updated()
    {
        ToggleSliderConfig tsc;
        if (!(custom_values & CustomFlags::ToggleSliderValues::Slider_Color))
        {
            slider_color_ = tsc.slider_color();
            meshes[1].update_color(slider_color_);
        }
        if (!(custom_values & CustomFlags::ToggleSliderValues::Hover_Color))
        {
            hover_color_ = tsc.hover_color();
        }
        if (!(custom_values & CustomFlags::ToggleSliderValues::Background_Color))
        {
            background_color_ = tsc.background_color();
            meshes[0].update_color(background_color_);
        }
        if (!(custom_values & CustomFlags::Radius))
        {
            config.radius = tsc.radius();
            meshes[0].update_radius(config.radius);
        }
    }

    void ToggleSlider::self_scale_updated(glm::vec2 new_scale)
    {
        handle = AnimationHandle();

        meshes[0].update_size(config.size);
        float inset = (config.size.y * 0.5f) - 4.f;
        meshes[1].update_size(glm::vec2(inset * 2.f));
        meshes[0].update_radius(glm::vec4(inset + 4.f));
        meshes[1].update_radius(glm::vec4(inset));

        if (toggled)
        {
            meshes[1].update_position({ get_position().x + config.size.x - (meshes[1].get_size().x + 4.f), meshes[1].get_position().y });
        }
        else
        {
            meshes[1].update_position({ get_position().x + 4.f, meshes[1].get_position().y });
        }
        mark_dirty();
    }

    void ToggleSlider::self_update(float delta_time)
    {
    }

    void ToggleSlider::self_on_mouse_button(UI::MouseButton button, UI::Action action)
    {
        if (!check_state(IS_HOVERED))
            return;
        if (action == UI::Action::Press && button == UI::MouseButton::Left)
        {
            toggled = !toggled;
            if (bound_variable)
            {
                *bound_variable = toggled;
            }
            handle = Animate::Start([this](float x)
            {
                glm::vec2 pos = meshes[1].get_position();
                pos.x = std::lerp(get_position().x + 4.f, get_position().x + config.size.x - (meshes[1].get_size().x + 4.f), x);
                meshes[1].update_position(pos);
                mark_dirty();
            }, 0.15f, AnimationType::Perlin_SmoothStep, toggled ? AnimDirection::Forward : AnimDirection::Backward);
        }
    }

    ToggleSliderConfig::ToggleSliderConfig()
    {
        if (!ThemeManager::get().is_loaded())
        {
            return;
        }
        if (JSONToken* toggle_slider = ThemeManager::get().try_get("Toggle Slider"))
        {
            for (const auto& [name, value] : toggle_slider->as_object())
            {
                if (name == "Slider Color")
                {
                    slider_color_ = value.as_vec4();
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
                    radius_ = value.as_vec4();
                }
            }
        }
    }

    ToggleSliderConfig& ToggleSliderConfig::slider_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::ToggleSliderValues::Slider_Color;
        slider_color_ = color;
        return *this;
    }

    ToggleSliderConfig& ToggleSliderConfig::hover_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::ToggleSliderValues::Hover_Color;
        hover_color_ = color;
        return *this;
    }

    ToggleSliderConfig& ToggleSliderConfig::background_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::ToggleSliderValues::Background_Color;
        background_color_ = color;
        return *this;
    }

    static LayoutRegistry::AutoRegister dummy("ToggleSlider", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
    {
        ToggleSliderConfig config;
        config.id(props.Id.value());
        if (props.Pos)
            config.position(props.Pos.value());
        if (props.Size)
            config.size(props.Size.value());
        if (props.Radius)
            config.radius(props.Radius.value());
        if (props.Flex)
            config.flex(props.Flex.value());

        if (auto it = json_object.find("Slider Color"); it != json_object.end())
        {
            config.slider_color(it->second.as_vec4());
        }
        if (auto it = json_object.find("Background Color"); it != json_object.end())
        {
            config.background_color(it->second.as_vec4());
        }
        if (auto it = json_object.find("Hover Color"); it != json_object.end())
        {
            config.hover_color(it->second.as_vec4());
        }
        return std::make_unique<ToggleSlider>(config);
    });
} // namespace TempoUI
