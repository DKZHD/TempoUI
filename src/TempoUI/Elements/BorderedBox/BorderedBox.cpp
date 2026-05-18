#include "BorderedBox.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "UIMesh/UIMesh.h"
#include "ThemeManager/ThemeManager.h"
#include "JsonWriter/JsonWriter.h"

namespace TempoUI
{
    BorderedBox::BorderedBox(const BorderedBoxConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex()), inset(_config.inset())
    {
        glm::vec4 inner_radius = glm::max(_config.radius() - glm::vec4(inset), glm::vec4(0.f));

        config.localPosition = _config.position();
        config.size = _config.size();
        config.state |= HOVERABLE;
        config.state |= IS_CONTAINER;
        config.radius = _config.radius();

        meshes.reserve(2);
        meshes.emplace_back(get_position(), config.size, _config.outer_color(), _config.radius());
        meshes.emplace_back(get_position() + inset, config.size - (inset * 2.f), _config.inner_color(), inner_radius);
    }

    BorderedBox::BorderedBox(const std::string& id, glm::vec2 pos, glm::vec2 size, float inset, glm::vec4 radius) : ElementBase(id), inset(inset)
    {
        glm::vec4 inner_radius = glm::max(radius - glm::vec4(inset), glm::vec4(0.f));

        config.localPosition = pos;
        config.size = size;
        config.state |= HOVERABLE;
        config.state |= IS_CONTAINER;
        config.radius = radius;

        meshes.reserve(2);
        meshes.emplace_back(get_position(), size, glm::vec4(0.f, 0.f, 0.f, 1.f), radius);
        meshes.emplace_back(get_position() + inset, size - (inset * 2.f), glm::vec4(1.f), inner_radius);
    }

    BorderedBox::~BorderedBox()
    {
    }

    void BorderedBox::set_outer_color(glm::vec4 color)
    {
        meshes[0].update_color(color);
        mark_dirty();
    }

    void BorderedBox::set_inner_color(glm::vec4 color)
    {
        meshes[1].update_color(color);
        mark_dirty();
    }

    void BorderedBox::self_theme_updated()
    {
        BorderedBoxConfig _config = BorderedBoxConfig().id(ID).position(config.localPosition).size(config.size); // NOLINT

        if (!(custom_values & CustomFlags::Outer_Color))
        {
            meshes[0].update_color(_config.outer_color());
        }
        if (!(custom_values & CustomFlags::Inner_Color))
        {
            meshes[1].update_color(_config.inner_color());
        }
        if (!(custom_values & CustomFlags::Inset))
        {
            inset = _config.inset();
        }
        if (!(custom_values & CustomFlags::Radius))
        {
            config.radius = _config.radius();
        }

        glm::vec4 inner_radius = glm::max(config.radius - glm::vec4(inset), glm::vec4(0.f));

        meshes[0].update_radius(config.radius);
        meshes[1].update_radius(inner_radius);
        meshes[1].update_size(config.size - (inset * 2.f));
    }

    void BorderedBox::self_update(float dt)
    {
    }

    void BorderedBox::update_buffers()
    {
        vertices.clear();
        indices.clear();

        if (parent != nullptr)
        {
            meshes[0].update_position(get_position());
            meshes[1].update_position(get_position() + inset);

            meshes[0].update_size(config.size);
            meshes[1].update_size(config.size - (inset * 2.f));
        }

        vertices.reserve(meshes.size() * 4);
        indices.reserve(meshes.size() * 6);

        for (UIMesh& mesh : meshes)
        {
            for (unsigned int index : mesh.get_indices())
            {
                indices.push_back(index + vertices.size());
            }
            vertices.insert(vertices.end(), mesh.get_vertices().begin(), mesh.get_vertices().end());
        }

        for (const auto& child : children)
        {
            child->update_absolute_position();
            child->update_buffers();
        }
    }

    BorderedBoxConfig::BorderedBoxConfig()
    {
        if (!ThemeManager::get().is_loaded())
        {
            return;
        }
        if (JSONToken* bordered_box = ThemeManager::get().try_get("Bordered Box"))
        {
            const std::map<std::string, JSONToken>& mapped_values = bordered_box->as_object();
            for (const auto& [name, value] : mapped_values)
            {
                if (name == "Outer Color")
                {
                    outer_color_ = (value.as_vec4());
                }
                else if (name == "Inner Color")
                {
                    inner_color_ = (value.as_vec4());
                }
                else if (name == "Inset")
                {
                    inset_ = ((float)value.as_number());
                }
                else if (name == "Radius")
                {
                    radius_ = (value.as_vec4());
                }
                else if (name == "Size")
                {
                    size_ = (value.as_vec2());
                }
            }
        }
    }

    void BorderedBox::self_serialize(JsonWriter& writer)
    {
        writer.write_string("Type", "BorderedBox");
        writer.write_vec4("Inner Color", meshes[1].get_color());
        writer.write_vec4("Outer Color", meshes[0].get_color());
        writer.write_float("Inset", inset);
    }

    static LayoutRegistry::AutoRegister dummy("BorderedBox", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
    {
        BorderedBoxConfig config;
        config.id(props.Id.value());
        if (props.Pos)
            config.position(props.Pos.value());
        if (props.Size)
            config.size(props.Size.value());
        if (props.Radius)
            config.radius(props.Radius.value());
        if (props.Flex)
            config.flex(props.Flex.value());

        if (auto inner_color = json_object.find("Inner Color"); inner_color != json_object.end())
        {
            config.inner_color(inner_color->second.as_vec4());
        }
        if (auto outer_color = json_object.find("Outer Color"); outer_color != json_object.end())
        {
            config.outer_color(outer_color->second.as_vec4());
        }
        if (auto inset_it = json_object.find("Inset"); inset_it != json_object.end())
        {
            config.inset((float)inset_it->second.as_number());
        }
        return std::make_unique<BorderedBox>(config);
    });
} // namespace TempoUI
