#include "Shader.h"
#include "FileReader/FileReader.h"
#include "ImageHandler/ImageHandler.h"
#include "TextHandler/TextHandler.h"

namespace
{
    void checkCompileErrors(unsigned int shader, std::string_view type)
    {
        int success = 0;
        char info_log[1024];

        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, nullptr, info_log);
                std::cout << info_log << '\n';
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, nullptr, info_log);
                std::cout << info_log << '\n'; // NOLINT
            }
        }
    }
} // namespace

Shader::Shader(std::string_view vert, std::string_view frag, bool fromFile)
{
    unsigned int vert_shader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vert_src;
    std::string frag_src;

    if (fromFile)
    {
        vert_src = FileReader::read_File(vert);
        frag_src = FileReader::read_File(frag);
    }
    else
    {
        vert_src = vert;
        frag_src = frag;
    }

    const char* vert_src_char = vert_src.c_str();
    glShaderSource(vert_shader, 1, &vert_src_char, nullptr);
    glCompileShader(vert_shader);
    checkCompileErrors(vert_shader, "VERTEX");

    const char* frag_src_char = frag_src.c_str();
    glShaderSource(frag_shader, 1, &frag_src_char, nullptr);
    glCompileShader(frag_shader);
    checkCompileErrors(frag_shader, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vert_shader);
    glAttachShader(ID, frag_shader);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    use();
    int samplers[32]; // NOLINT
    for (int i = 0; i < 32; i++)
    {
        samplers[i] = i; // NOLINT
    }
    glUniform1iv(static_cast<GLint>(get_uniform_location("uTextures")), 32, samplers);
}

Shader::~Shader()
{
    if (ID)
    {
        glDeleteProgram(ID);
    }
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::set_int(const std::string& name, int i)
{
    glUniform1i(static_cast<GLint>(get_uniform_location(name)), i);
}
void Shader::set_float(const std::string& name, float f)
{
    glUniform1f(static_cast<GLint>(get_uniform_location(name)), f);
}

void Shader::set_texture(int index, unsigned int texture) // NOLINT
{
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Shader::set_vec2(const std::string& name, glm::vec2 v)
{
    glUniform2fv(static_cast<GLint>(get_uniform_location(name)), 1, &v[0]);
}

void Shader::set_vec3(const std::string& name, glm::vec3 v)
{
    glUniform3fv(static_cast<GLint>(get_uniform_location(name)), 1, &v[0]);
}

void Shader::set_vec4(const std::string& name, glm::vec4 v)
{
    glUniform4fv(static_cast<GLint>(get_uniform_location(name)), 1, &v[0]);
}

void Shader::set_mat4(const std::string& name, glm::mat4 m)
{
    glUniformMatrix4fv(static_cast<GLint>(get_uniform_location(name)), 1, GL_FALSE, &m[0][0]);
}

void Shader::load_images()
{
    ImageHandler::get().load_images(this);
}

void Shader::add_image(const std::string& name) // NOLINT
{
    ImageHandler::get().add_texture(name);
}

void Shader::add_image_from_memory(const std::string& name, const unsigned char* data, int length) // NOLINT
{
    ImageHandler::get().add_texture_from_data(name, data, length);
}

void Shader::add_texture(const std::string& name, uint32_t texture) // NOLINT
{
    ImageHandler::get().add_texture(name, texture);
}

void Shader::add_texture(const std::string& name, const std::string& path) // NOLINT
{
    ImageHandler::get().add_texture(name, path);
}

uint32_t Shader::load_font(const std::string& font_name, const std::string& path, unsigned int font_size) // NOLINT
{
    return TextHandler::get().load_font(font_name, path, font_size);
}

uint32_t Shader::load_font_from_memory(const std::string& font_name, const unsigned char* data, long length, unsigned int font_size) // NOLINT
{
    return TextHandler::get().load_font_from_memory(font_name, data, length, font_size);
}

unsigned Shader::get_uniform_location(const std::string& name)
{
    auto it = uniforms.find(name);
    if (it != uniforms.end())
    {
        return it->second;
    }
    uniforms[name] = glGetUniformLocation(ID, name.c_str());
    return uniforms.at(name);
}
