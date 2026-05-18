#include "ImageHandler.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader/Shader.h"

namespace TempoUI
{
    namespace
    {
        uint32_t load_image(const std::string& path)
        {
            int x = 0;
            int y = 0;
            int num_channels = 0;

            unsigned char* data = stbi_load(path.c_str(), &x, &y, &num_channels, 4);
            if (data == nullptr)
            {
                std::cout << "Failed to load image: " << path << '\n';
                return UINT32_MAX;
            }

            uint32_t tex = UINT32_MAX;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

            stbi_image_free(data);

            return tex;
        }
        uint32_t load_image_from_memory(const unsigned char* data, int length)
        {
            int x = 0;
            int y = 0;
            int num_channels = 0;
            if (data == nullptr)
            {
                std::cout << "Failed to load image from memory!" << '\n';
                return UINT32_MAX;
            }
            stbi_load_from_memory(data, length, &x, &y, &num_channels, 4);

            uint32_t tex = UINT32_MAX;
            glGenTextures(1, &tex);
            glBindTexture(GL_TEXTURE_2D, tex);

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

            return tex;
        }
    } // namespace

    ImageHandler::ImageHandler()
    {
    }

    ImageHandler::~ImageHandler()
    {
    }

    void ImageHandler::load_images(Shader* shader)
    {
        for (int i = 0; i < textures.size(); i++)
        {
            shader->set_texture(i, textures[i]);
        }
    }

    void ImageHandler::add_texture(const std::string& name)
    {
        if (texture_indexes.contains(name))
        {
            std::cout << "Texture already exists: " << name << '\n';
            return;
        }
        texture_indexes.insert({ name, textures.size() });
        textures.emplace_back(load_image(name));
    }

    void ImageHandler::add_texture(const std::string& name, unsigned int texture)
    {
        if (texture_indexes.contains(name))
        {
            std::cout << "Texture already exists: " << name << '\n';
            return;
        }
        texture_indexes.insert({ name, textures.size() });
        textures.emplace_back(texture);
    }

    void ImageHandler::add_texture(const std::string& name, const std::string& path)
    {
        if (texture_indexes.contains(name))
        {
            std::cout << "Texture already exists: " << name << '\n';
            return;
        }
        texture_indexes.insert({ name, textures.size() });
        textures.emplace_back(load_image(path));
    }

    void ImageHandler::add_texture_from_data(const std::string& name, const unsigned char* data, int length)
    {
        if (texture_indexes.contains(name))
        {
            std::cout << "Texture already exists: " << name << '\n';
            return;
        }
        texture_indexes.insert({ name, textures.size() });
        textures.emplace_back(load_image_from_memory(data, length));
    }

    uint32_t ImageHandler::get_texture(const std::string& name) const
    {
        auto it = texture_indexes.find(name);
        if (it != texture_indexes.end())
            return it->second;
        return 32;
    }

    ImageHandler& ImageHandler::get()
    {
        static ImageHandler instance;
        return instance;
    }
} // namespace TempoUI
