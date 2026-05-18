#pragma once

namespace TempoUI
{
    template <void (*DeleteFunc)(int, const unsigned*)>
    class GLResource
    {
    public:
        GLResource() = default;
        ~GLResource()
        {
            if (ID)
                DeleteFunc(1, &ID);
        }

        GLResource(const GLResource&) = delete;
        GLResource& operator=(const GLResource&) = delete;
        GLResource(GLResource&& other) noexcept : ID(other.ID)
        {
            other.ID = 0;
        }

        GLResource& operator=(GLResource&& other) noexcept
        {
            if (this != &other)
            {
                if (ID)
                    DeleteFunc(1, &ID);

                ID = other.ID;
                other.ID = 0;
            }
            return *this;
        }

        operator unsigned() const noexcept
        {
            return ID;
        }

        operator unsigned*() noexcept
        {
            return &ID;
        }

    private:
        unsigned ID = 0;
    };

    inline void delBuf(int n, const unsigned* buffer) { glDeleteBuffers(n, buffer); }
    inline void delVAO(int n, const unsigned* array) { glDeleteVertexArrays(n, array); }
    inline void delTexture(int n, const unsigned* texture) { glDeleteTextures(n, texture); }

    using GLBuffer = GLResource<delBuf>;
    using GLVertexArray = GLResource<delVAO>;
    using GLTexture = GLResource<delTexture>;
} // namespace TempoUI
