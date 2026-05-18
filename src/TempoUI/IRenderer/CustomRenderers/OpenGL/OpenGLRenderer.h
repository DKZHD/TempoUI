#pragma once

#include "GLResource/GLResource.h"
#include "IRenderer/IRenderer.h"

namespace TempoUI
{
    class Shader;

    class OpenGLRenderer : public IRenderer
    {
    public:
        OpenGLRenderer(Shader& shader);
        void init(void* window_handle) override;
        void begin_frame() override;
        void end_frame() override;

        void set_scissor(int x, int y, int width, int height, int window_height) override;
        void disable_scissor() override;

        void draw_indexed(size_t start_index, size_t count) override;
        void update_buffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) override;

    private:
        GLVertexArray vao_;
        GLBuffer vbo_;
        GLBuffer ebo_;
        Shader& shader_;
    };
} // namespace TempoUI
