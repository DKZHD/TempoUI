#pragma once
#include "Elements/ElementBase/ElementBase.h"
#include "Elements/Text/Text.h"
#include "UIMesh/UIMesh.h"

namespace CustomFlags
{
    enum class GraphViewValues : uint32_t // NOLINT
    {
        Node_Color = 1 << 8,
        Background_Color = 1 << 9,
        Inner_Color = 1 << 10,
        Scope_Size = 1 << 11,
        Interval = 1 << 12,
        Min = 1 << 13,
        Max = 1 << 14
    };

    inline uint32_t& operator|=(uint32_t& val, GraphViewValues right)
    {
        val |= (uint32_t)right;
        return val;
    }
    inline bool operator&(uint32_t val, GraphViewValues right)
    {
        return val & (uint32_t)right;
    }
} // namespace CustomFlags

class GraphViewConfig : public ElementConfig<GraphViewConfig>
{
public:
    GraphViewConfig();
    GraphViewConfig& node_color(glm::vec4 color);
    GraphViewConfig& background_color(glm::vec4 color);
    GraphViewConfig& inner_color(glm::vec4 color);
    GraphViewConfig& scope_size(int _size);
    GraphViewConfig& interval(float _interval);
    GraphViewConfig& min(float _min);
    GraphViewConfig& max(float _max);

    [[nodiscard]] glm::vec4 node_color() const { return node_color_; }
    [[nodiscard]] glm::vec4 background_color() const { return background_color_; }
    [[nodiscard]] glm::vec4 inner_color() const { return inner_color_; }
    [[nodiscard]] int scope_size() const { return scope_size_; }
    [[nodiscard]] float interval() const { return interval_; }
    [[nodiscard]] float min() const { return min_; }
    [[nodiscard]] float max() const { return max_; }

private:
    glm::vec4 node_color_ = glm::vec4(0.f, 0.f, 0.f, 1.f);
    glm::vec4 background_color_ = glm::vec4(0.f, 0.f, 0.f, 1.f);
    glm::vec4 inner_color_ = glm::vec4(1.f);
    int scope_size_ = 10;
    float interval_ = 1.f;
    float min_ = 0.f;
    float max_ = 10.f;
};

class GraphView : public ElementBase
{
public:
    GraphView(const GraphViewConfig& _config);
    GraphView(const std::string& id, glm::vec2 pos, glm::vec2 size, int scope_size, float interval, float min, float max);
    void bind_variable(float* var);

private:
    void update_buffers() override;
    void self_update(float delta_time) override;
    void self_scale_updated(glm::vec2 new_scale) override;

    struct GraphNode
    {
        UIMesh mesh;
    };

    std::vector<float> values;
    std::vector<GraphNode> nodes;
    std::array<UIMesh, 2> meshes_;

    float timer_ = 0.f;
    float interval_ = -1.f;
    float min_ = 0.f;
    float max_ = 1.f;
    float* bound_variable = nullptr;
};