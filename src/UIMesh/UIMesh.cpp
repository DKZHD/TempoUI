#include "UIMesh.h"

UIMesh::UIMesh(glm::vec2 pos, glm::vec2 size, glm::vec4 color, glm::vec4 radius, float textureID)
{
    vertices[0].position = pos;
    vertices[1].position = pos + glm::vec2(0.f, size.y);
    vertices[2].position = pos + glm::vec2(size);
    vertices[3].position = pos + glm::vec2(size.x, 0.f);

    indices = { 0, 1, 2, 2, 3, 0 };

    for (int i = 0; i < 4; i++)
    {
        vertices[i].size = size;
        vertices[i].color = color;
        vertices[i].radius = radius;
        vertices[i].textureID = textureID;
    }

    update_texture_coords(glm::vec2(0.f), glm::vec2(1.f));
}

UIMesh::~UIMesh()
{
}

void UIMesh::update_texture_coords(glm::vec2 upper_left, glm::vec2 bottom_right)
{
    vertices[0].tex_coords = upper_left;
    vertices[1].tex_coords = glm::vec2(upper_left.x, bottom_right.y);
    vertices[2].tex_coords = bottom_right;
    vertices[3].tex_coords = glm::vec2(bottom_right.x, upper_left.y);
}

void UIMesh::update_texture_id(unsigned int tex_id)
{
    for (Vertex& vertex : vertices)
    {
        vertex.textureID = static_cast<float>(tex_id);
    }
}

const std::array<Vertex, 4>& UIMesh::get_vertices()
{
    return vertices;
}

const std::array<unsigned int, 6>& UIMesh::get_indices()
{
    return indices;
}

void UIMesh::update_color(glm::vec4 color)
{
    for (Vertex& vertex : vertices)
    {
        vertex.color = color;
    }
}

glm::vec4 UIMesh::get_color() const
{
    return vertices[0].color;
}

void UIMesh::update_position(glm::vec2 position)
{
    glm::vec2 size = vertices[0].size;

    vertices[0].position = position;
    vertices[1].position = position + glm::vec2(0.f, size.y);
    vertices[2].position = position + glm::vec2(size);
    vertices[3].position = position + glm::vec2(size.x, 0.f);
}

glm::vec2 UIMesh::get_position() const
{
    return vertices[0].position;
}

void UIMesh::update_radius(glm::vec4 radius)
{
    vertices[0].radius = radius;
    vertices[1].radius = radius;
    vertices[2].radius = radius;
    vertices[3].radius = radius;
}

glm::vec4 UIMesh::get_radius() const
{
    return vertices[0].radius;
}

void UIMesh::update_size(glm::vec2 size)
{
    vertices[1].position = vertices[0].position + glm::vec2(0.f, size.y);
    vertices[2].position = vertices[0].position + glm::vec2(size);
    vertices[3].position = vertices[0].position + glm::vec2(size.x, 0.f);

    for (Vertex& vertex : vertices)
    {
        vertex.size = size;
    }
}

glm::vec2 UIMesh::get_size() const
{
    return vertices[0].size;
}
