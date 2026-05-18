#pragma once

namespace TempoUI
{
    struct Vertex
    {
        glm::vec2 position = glm::vec2(0.f);
        glm::vec2 tex_coords = glm::vec2(0.f);
        glm::vec2 size = glm::vec2(0.f);
        glm::vec4 color = glm::vec4(1.f);
        glm::vec4 radius = glm::vec4(0.f);
        float textureID = 0.f;
    };

    class UIMesh
    {
    public:
        UIMesh() = default;
        UIMesh(glm::vec2 pos, glm::vec2 size, glm::vec4 color = glm::vec4(1.f), glm::vec4 radius = glm::vec4(0.f), float textureID = 0.f);
        ~UIMesh();

        void update_texture_coords(glm::vec2 upper_left, glm::vec2 bottom_right);
        void update_texture_id(unsigned int tex_id);
        const std::array<Vertex, 4>& get_vertices();
        const std::array<unsigned int, 6>& get_indices();
        void update_color(glm::vec4 color);
        glm::vec4 get_color() const;
        void update_position(glm::vec2 position);
        glm::vec2 get_position() const;
        void update_radius(glm::vec4 radius);
        glm::vec4 get_radius() const;
        void update_size(glm::vec2 size);
        [[nodiscard]] glm::vec2 get_size() const;

    private:
        std::array<Vertex, 4> vertices = {};
        std::array<unsigned int, 6> indices = {};
    };
} // namespace TempoUI
