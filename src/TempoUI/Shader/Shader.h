#pragma once

namespace TempoUI
{
    class TextHandler;
    class ImageHandler;

    class Shader
    {
    public:
        Shader() = default;
        Shader(std::string_view vert, std::string_view frag, bool fromFile = true);
        ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        void use() const;

        void set_int(const std::string& name, int i);
        void set_float(const std::string& name, float f);
        void set_texture(int index, unsigned int texture);
        void set_vec2(const std::string& name, glm::vec2 v);
        void set_vec3(const std::string& name, glm::vec3 v);
        void set_vec4(const std::string& name, glm::vec4 v);
        void set_mat4(const std::string& name, glm::mat4 m);
        void load_images();
        void add_image(const std::string& name);
        void add_image_from_memory(const std::string& name, const unsigned char* data, int length);

        void add_texture(const std::string& name, uint32_t texture);
        void add_texture(const std::string& name, const std::string& path);

        uint32_t load_font(const std::string& font_name, const std::string& path, unsigned int font_size);
        uint32_t load_font_from_memory(const std::string& font_name, const unsigned char* data, long length, unsigned int font_size);

        operator unsigned() const
        {
            return ID;
        }

    private:
        unsigned ID = 0;
        unsigned get_uniform_location(const std::string& name);
        std::unordered_map<std::string, unsigned> uniforms;
    };
} // namespace TempoUI
