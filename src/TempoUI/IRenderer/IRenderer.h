#pragma once

namespace TempoUI
{
    struct Vertex;

    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        virtual void init(void* window_handle) = 0;
        virtual void begin_frame() = 0;
        virtual void end_frame() = 0;

        virtual void set_scissor(int x, int y, int width, int height, int window_height) = 0;
        virtual void disable_scissor() = 0;

        virtual void draw_indexed(size_t start_index, size_t count) = 0;
        virtual void update_buffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) = 0;
    };
} // namespace TempoUI
