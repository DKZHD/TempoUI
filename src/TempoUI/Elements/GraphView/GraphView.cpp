#include "GraphView.h"
#include "Elements/BorderedBox/BorderedBox.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "ThemeManager/ThemeManager.h"

namespace TempoUI
{
    GraphViewConfig::GraphViewConfig()
    {
        if (JSONToken* graph_view = ThemeManager::get().try_get("GraphView"))
        {
            for (const auto& [key, val] : graph_view->as_object())
            {
                if (key == "Node Color")
                {
                    node_color_ = val.as_vec4();
                }
                else if (key == "Background Color")
                {
                    background_color_ = val.as_vec4();
                }
                else if (key == "Inner Color")
                {
                    inner_color_ = val.as_vec4();
                }
                else if (key == "Radius")
                {
                    radius_ = (val.as_vec4());
                }
            }
        }
    }

    GraphViewConfig& GraphViewConfig::node_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::GraphViewValues::Node_Color;
        node_color_ = color;
        return *this;
    }
    GraphViewConfig& GraphViewConfig::background_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::GraphViewValues::Background_Color;
        background_color_ = color;
        return *this;
    }
    GraphViewConfig& GraphViewConfig::inner_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::GraphViewValues::Inner_Color;
        inner_color_ = color;
        return *this;
    }
    GraphViewConfig& GraphViewConfig::scope_size(int _size)
    {
        custom_values |= CustomFlags::GraphViewValues::Scope_Size;
        scope_size_ = _size;
        return *this;
    }
    GraphViewConfig& GraphViewConfig::interval(float _interval)
    {
        custom_values |= CustomFlags::GraphViewValues::Interval;
        interval_ = _interval;
        return *this;
    }
    GraphViewConfig& GraphViewConfig::min(float _min)
    {
        custom_values |= CustomFlags::GraphViewValues::Min;
        min_ = _min;
        return *this;
    }
    GraphViewConfig& GraphViewConfig::max(float _max)
    {
        custom_values |= CustomFlags::GraphViewValues::Max;
        max_ = _max;
        return *this;
    }

    GraphView::GraphView(const GraphViewConfig& _config)
        : ElementBase(_config.id(), _config.get_flags(), _config.flex()), interval_(_config.interval()), min_(_config.min()), max_(_config.max())
    {
        config.size = _config.size();
        config.localPosition = _config.position();
        config.radius = _config.radius();

        values.resize(_config.scope_size(), 0.5f);
        nodes.reserve(_config.scope_size());

        float x_extra = 1.f / (float)_config.scope_size();
        float x_offset = ((config.size.x - 10.f) / (float)_config.scope_size());

        meshes_[0] = UIMesh(get_position(), config.size, _config.background_color(), glm::vec4(_config.radius()));
        meshes_[1] = UIMesh(get_position() + 4.f, { config.size - 8.f }, _config.inner_color(), glm::vec4(_config.radius() - 4.f));

        for (int i = 0; i < _config.scope_size(); i++)
        {
            float x_pos = get_position().x + 5.f + (x_offset * (float)i);
            nodes.emplace_back(UIMesh({ x_pos + x_extra, 0.f }, { x_offset - 1.f, 0.f }, _config.node_color(), glm::vec4((x_offset - 1.f) * 0.1f)));
        }
    }

    GraphView::GraphView(const std::string& id, glm::vec2 pos, glm::vec2 size, int scope_size, float interval, float min, float max)
        : ElementBase(id), interval_(interval), min_(min), max_(max)
    {
        config.size = size;
        config.localPosition = pos;

        values.resize(scope_size, 0.f);
        nodes.reserve(scope_size);

        float x_offset = (size.x - 10.f) / (float)scope_size;
        float x_extra = 1.f / (float)scope_size;

        meshes_[0] = UIMesh(get_position(), size, glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec4((x_offset - 1.f) * 0.15f) + 4.f);
        meshes_[1] = UIMesh(get_position() + 4.f, { size - 8.f }, glm::vec4(1.f), glm::vec4((x_offset - 1.f) * 0.15f));

        for (int i = 0; i < scope_size; i++)
        {
            float x_pos = get_position().x + 5.f + (x_offset * (float)i);
            nodes.emplace_back(UIMesh({ x_pos + x_extra, 0.f }, { x_offset - 1.f, 0.f }, glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec4((x_offset - 1.f) * 0.15f)));
        }
    }

    void GraphView::bind_variable(float* var)
    {
        bound_variable = var;
    }

    void GraphView::update_buffers()
    {
        vertices.clear();
        indices.clear();

        vertices.reserve(nodes.size() * 4);
        indices.reserve(nodes.size() * 6);

        meshes_[0].update_position(get_position());
        meshes_[1].update_position(get_position() + 4.f);

        for (UIMesh& mesh : meshes_)
        {
            const std::array<Vertex, 4>& background_vertices = mesh.get_vertices();
            const std::array<unsigned int, 6>& background_indices = mesh.get_indices();

            for (unsigned int index : background_indices)
            {
                indices.emplace_back(index + vertices.size());
            }
            vertices.insert(vertices.end(), background_vertices.begin(), background_vertices.end());
        }

        for (int i = 0; i < nodes.size(); i++)
        {
            float value = values[i];
            GraphNode& node = nodes[i];

            float percent = std::clamp((value - min_) / (max_ - min_), 0.f, 1.f);
            float current_pos_x = node.mesh.get_position().x;

            node.mesh.update_position(glm::vec2(current_pos_x, get_position().y + (config.size.y - 5.f) - ((config.size.y - 10.f) * percent)));
            node.mesh.update_size({ node.mesh.get_size().x, ((percent * (config.size.y - 10.f))) });

            const std::array<Vertex, 4>& mesh_vertices = node.mesh.get_vertices();
            const std::array<unsigned int, 6>& mesh_indices = node.mesh.get_indices();

            for (unsigned int index : mesh_indices)
            {
                indices.emplace_back(index + vertices.size());
            }
            vertices.insert(vertices.end(), mesh_vertices.begin(), mesh_vertices.end());
        }
    }

    void GraphView::self_update(float delta_time)
    {
        timer_ += delta_time;

        if (interval_ > 0.f && timer_ > interval_ && bound_variable)
        {
            float prev_val = *bound_variable;
            for (int i = (int)values.size() - 1; i >= 0; i--)
            {
                float temp = prev_val;
                prev_val = values[i];
                values[i] = temp;
            }
            mark_dirty();
            timer_ = 0.f;
        }
    }

    void GraphView::self_scale_updated(glm::vec2 new_scale)
    {
        float x_offset = ((config.size.x - 10.f) / (float)values.size());

        meshes_[0].update_position(get_position());
        meshes_[1].update_position(get_position() + 4.f);
        meshes_[0].update_size(config.size);
        meshes_[1].update_size(config.size - 8.f);
        meshes_[0].update_radius(config.radius);
        meshes_[1].update_radius(config.radius - 4.f);

        for (int i = 0; i < nodes.size(); i++)
        {
            GraphNode& node = nodes[i];
            float current_pos_x = get_position().x + (x_offset * (float)i) + 5.f;

            node.mesh.update_position(glm::vec2(current_pos_x, 0.f));
            node.mesh.update_size({ x_offset - 1.f, 0.f });
        }
    }

    static LayoutRegistry::AutoRegister dummy("GraphView", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
    {
        GraphViewConfig config;
        config.id(props.Id.value());
        if (props.Pos)
            config.position(props.Pos.value());
        if (props.Size)
            config.size(props.Size.value());
        if (props.Radius)
            config.radius(props.Radius.value());
        if (props.Flex)
            config.flex(props.Flex.value());

        if (auto it = json_object.find("Node Color"); it != json_object.end())
        {
            config.node_color(it->second.as_vec4());
        }
        if (auto it = json_object.find("Background Color"); it != json_object.end())
        {
            config.background_color(it->second.as_vec4());
        }
        if (auto it = json_object.find("Inner Color"); it != json_object.end())
        {
            config.inner_color(it->second.as_vec4());
        }
        if (auto it = json_object.find("Scope Size"); it != json_object.end())
        {
            config.scope_size((int)it->second.as_number());
        }
        if (auto it = json_object.find("Interval"); it != json_object.end())
        {
            config.interval((float)it->second.as_number());
        }
        if (auto it = json_object.find("Min"); it != json_object.end())
        {
            config.min((float)it->second.as_number());
        }
        if (auto it = json_object.find("Max"); it != json_object.end())
        {
            config.max((float)it->second.as_number());
        }
        return std::make_unique<GraphView>(config);
    });
} // namespace TempoUI
