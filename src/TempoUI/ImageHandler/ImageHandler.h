#pragma once

namespace TempoUI
{
    class Shader;

    class ImageHandler
    {
    public:
        static ImageHandler& get();

        void load_images(Shader* shader);

        // Include file extension (.png, .jpg etc.)
        void add_texture(const std::string& name);
        void add_texture(const std::string& name, const std::string& path);
        void add_texture_from_data(const std::string& name, const unsigned char* data, int length);

        void add_texture(const std::string& name, unsigned int texture);

        // Includes file extension (.png, .jpg etc.)
        uint32_t get_texture(const std::string& name) const;

    private:
        ImageHandler();
        ~ImageHandler();
        std::unordered_map<std::string, size_t> texture_indexes;
        std::vector<uint32_t> textures;
    };
} // namespace TempoUI
