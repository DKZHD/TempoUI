#include "ProgressBar.h"
#include "AnimationManager/AnimationManager.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "ThemeManager/ThemeManager.h"

namespace TempoUI
{
    ProgressBar::ProgressBar(const ProgressBarConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex()), percent_(_config.percent()), inset_(_config.inset())
    {
        config.size = _config.size();
        config.radius = _config.radius();
        config.localPosition = _config.position();

        glm::vec4 inner_radius = glm::max(_config.radius() - _config.inset(), glm::vec4(0.0f));
        meshes_[0] = UIMesh(_config.position(), config.size, _config.border_color(), config.radius);
        meshes_[1] = UIMesh(_config.position() + inset_, config.size - (inset_ * 2), _config.color(), inner_radius);

        set_percent(percent_);
    }

    ProgressBar::ProgressBar(const std::string& id, glm::vec2 pos, glm::vec2 size, float max, float inset, glm::vec4 color, glm::vec4 borderColor, glm::vec4 radius) : ElementBase(id), inset_(inset), max_(max)
    {
        config.size = size;
        config.radius = radius;
        config.localPosition = pos;

        glm::vec4 inner_radius = glm::max(radius - inset, glm::vec4(0.0f));
        meshes_[0] = UIMesh(pos, size, borderColor, radius);
        meshes_[1] = UIMesh(pos + inset, size - (inset * 2), color, inner_radius);
    }

    ProgressBar::~ProgressBar()
    {
    }

    void ProgressBar::set_percent(float _percent)
    {
        glm::vec2 start_size = meshes_[1].get_size();
        float start_percent = percent_;
        _percent = std::clamp(_percent, 0.f, 1.f);
        handle = Animate::Start([this, start_size, start_percent, _percent](float x)
        {
            glm::vec2 size = start_size;
            this->percent_ = std::lerp(start_percent, _percent, x);
            meshes_[1].update_size(glm::vec2(config.size.x * this->percent_ - (inset_ * 2.f), config.size.y - (inset_ * 2)));
            mark_dirty();
        }, 0.15f, AnimationType::Perlin_SmoothStep);
    }

    float ProgressBar::get_percent() const
    {
        return percent_;
    }

    void ProgressBar::bind_variable(float& value, float max)
    {
        max_ = max;
        attached_variable_ = &value;
        set_percent(value / max);
    }

    void ProgressBar::self_update(float dt)
    {
        if (attached_variable_ != nullptr)
        {
            float percent = *attached_variable_ / max_;
            if (std::abs(percent - target_percent) > 0.001f)
            {
                target_percent = percent;
                set_percent(target_percent);
            }
        }
    }

    void ProgressBar::self_theme_updated()
    {
        ProgressBarConfig pbc;
        if (!(custom_values & CustomFlags::ProgressBarValues::Border_Color))
        {
            meshes_[0].update_color(pbc.border_color());
        }
        if (!(custom_values & CustomFlags::ProgressBarValues::Color))
        {
            meshes_[1].update_color(pbc.color());
        }
        if (!(custom_values & CustomFlags::ProgressBarValues::Inset))
        {
            inset_ = pbc.inset();
        }
        if (!(custom_values & CustomFlags::Radius))
        {
            config.radius = pbc.radius();
            meshes_[0].update_radius(config.radius);
            meshes_[1].update_radius(config.radius - inset_);
        }
    }

    void ProgressBar::update_buffers()
    {
        vertices.clear();
        indices.clear();

        vertices.reserve(meshes_.size() * 4);
        indices.reserve(meshes_.size() * 6);

        meshes_[0].update_position(get_position());
        meshes_[1].update_position(get_position() + inset_);

        meshes_[0].update_size(config.size);

        for (UIMesh& mesh : meshes_)
        {
            std::array<unsigned int, 6> mesh_indices = mesh.get_indices();
            for (unsigned int index : mesh_indices)
            {
                indices.emplace_back(index + vertices.size());
            }

            std::array<Vertex, 4> mesh_vertices = mesh.get_vertices();

            vertices.insert(vertices.end(), mesh_vertices.begin(), mesh_vertices.end());
        }

        for (const auto& child : children)
        {
            child->update_absolute_position();
            child->update_buffers();
        }
    }

    ProgressBarConfig::ProgressBarConfig()
    {
        if (!ThemeManager::get().is_loaded())
            return;

        if (JSONToken* progress_bar = ThemeManager::get().try_get("ProgressBar"))
        {
            const std::map<std::string, JSONToken>& mapped_values = progress_bar->as_object();
            for (const auto& [name, value] : mapped_values)
            {
                if (name == "Inset")
                {
                    inset_ = (float)value.as_number();
                }
                else if (name == "Color")
                {
                    color_ = value.as_vec4();
                }
                else if (name == "Border Color")
                {
                    border_color_ = value.as_vec4();
                }
                else if (name == "Radius")
                {
                    radius_ = value.as_vec4();
                }
            }
        }
    }

    static LayoutRegistry::AutoRegister dummy("ProgressBar", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
    {
        ProgressBarConfig config;
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
        if (auto it = json_object.find("Border Color"); it != json_object.end())
        {
            config.border_color(it->second.as_vec4());
        }
        if (auto it = json_object.find("Inset"); it != json_object.end())
        {
            config.inset((float)it->second.as_number());
        }
        if (auto it = json_object.find("Max"); it != json_object.end())
        {
            config.max((float)it->second.as_number());
        }
        if (auto it = json_object.find("Percent"); it != json_object.end())
        {
            config.percent((float)it->second.as_number());
        }

        return std::make_unique<ProgressBar>(config);
    });
} // namespace TempoUI
