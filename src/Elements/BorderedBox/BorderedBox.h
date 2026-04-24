#pragma once
#include "Elements/ElementBase/ElementBase.h"
struct Vertex;
class UIMesh;
class JsonWriter;

namespace CustomFlags
{
    enum BorderedBoxValues : uint32_t // NOLINT
    {
        Outer_Color = 1 << 8,
        Inner_Color = 1 << 9,
        Inset = 1 << 10,
    };
}

class BorderedBoxConfig : public ElementConfig<BorderedBoxConfig>
{
public:
    BorderedBoxConfig();
    BorderedBoxConfig& outer_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::Outer_Color;
        outer_color_ = color;
        return *this;
    }
    BorderedBoxConfig& inner_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::Inner_Color;
        inner_color_ = color;
        return *this;
    }
    BorderedBoxConfig& inset(float inset)
    {
        custom_values |= CustomFlags::Inset;
        inset_ = inset;
        return *this;
    }

    [[nodiscard]] glm::vec4 outer_color() const { return outer_color_; }
    [[nodiscard]] glm::vec4 inner_color() const { return inner_color_; }
    [[nodiscard]] float inset() const { return inset_; }

private:
    glm::vec4 outer_color_ = glm::vec4(0.f, 0.f, 0.f, 1.f);
    glm::vec4 inner_color_ = glm::vec4(1.f);
    float inset_ = 0.f;
};

class BorderedBox : public ElementBase
{
public:
    BorderedBox(const BorderedBoxConfig& _config);
    BorderedBox(const std::string& id, glm::vec2 pos, glm::vec2 size, float inset = 0.f, glm::vec4 radius = glm::vec4(0.f));
    ~BorderedBox();

    void set_outer_color(glm::vec4 color);
    void set_inner_color(glm::vec4 color);

private:
    void self_update(float dt) override;
    void self_theme_updated() override;
    void update_buffers() override;
    void self_serialize(JsonWriter& writer) override;

    std::vector<UIMesh> meshes;
    float inset = 0.f;
};
