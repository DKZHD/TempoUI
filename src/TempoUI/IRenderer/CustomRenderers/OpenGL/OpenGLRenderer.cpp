#include "OpenGLRenderer.h"
#include "TextHandler/TextHandler.h"
#include "UIMesh/UIMesh.h"
#include "Shader/Shader.h"

namespace TempoUI
{
    void OpenGLRenderer::init(void* window_handle)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

        glGenVertexArrays(1, vao_);
        glGenBuffers(1, vbo_);
        glGenBuffers(1, ebo_);

        glBindVertexArray(vao_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned), nullptr, GL_DYNAMIC_DRAW);

        // NOLINTBEGIN
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, tex_coords)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, size)));
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, radius)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, textureID)));
        glBindVertexArray(0);
        // NOLINTEND

        uint32_t white_texture = UINT32_MAX;

        glGenTextures(1, &white_texture);
        glBindTexture(GL_TEXTURE_2D, white_texture);

        uint32_t white_pixel = UINT32_MAX;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white_pixel);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        shader_.add_texture("whiteTexture", white_texture);
    }

    void OpenGLRenderer::begin_frame()
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(vao_);
        uint32_t font_id = TextHandler::get().get_font("default_font");
        shader_.set_int("DefaultFontID", (int)font_id - 1);
    }

    void OpenGLRenderer::end_frame()
    {
    }

    void OpenGLRenderer::set_scissor(int x, int y, int width, int height, int window_height)
    {
        glEnable(GL_SCISSOR_TEST);
        int inv_y = window_height - y - height;

        glScissor(x, inv_y, width, height);
    }
    void OpenGLRenderer::disable_scissor()
    {
        glDisable(GL_SCISSOR_TEST);
    }

    void OpenGLRenderer::draw_indexed(size_t start_index, size_t count)
    {
        glDrawElements(GL_TRIANGLES, (int)count, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(unsigned int) * start_index));
    }

    void OpenGLRenderer::update_buffers(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
        glBindVertexArray(vao_);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, (long long)(vertices.size() * sizeof(Vertex)), vertices.data(), GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (long long)((unsigned)indices.size() * sizeof(unsigned)), indices.data(), GL_DYNAMIC_DRAW);
    }

    OpenGLRenderer::OpenGLRenderer(Shader& shader) : shader_(shader)
    {
    }
} // namespace TempoUI
