#pragma once

using FT_Library = struct FT_LibraryRec_*;
using FT_Face = struct FT_FaceRec_*;

namespace TempoUI
{
    struct Character
    {
        glm::vec2 uvOffset;
        glm::vec2 uvSize;
        glm::ivec2 Size;
        glm::ivec2 Bearing;
        float advance;
    };

    class TextHandler
    {
    public:
        static TextHandler& get();
        ~TextHandler();

        uint32_t load_font(const std::string& id, const std::string& path, unsigned int font_size);
        uint32_t load_font_from_memory(const std::string& id, const unsigned char* data, long length, unsigned int font_size);

        const std::array<Character, 128>& get_characters(const std::string& id);
        Character get_character(const std::string& id, char c);
        glm::vec2 calculate_size_of_text(const std::string& id, const std::string& text);
        float get_global_ascent(const std::string& id);
        float get_global_decent(const std::string& id);
        uint32_t get_font(const std::string& id);

    private:
        uint32_t load_chars_into_atlas(const std::string& id, FT_Library& lib, FT_Face& face);

        TextHandler() = default;
        std::unordered_map<std::string, unsigned int> textures;
        std::unordered_map<std::string, float> global_ascents;
        std::unordered_map<std::string, float> global_descents;
        std::unordered_map<std::string, std::array<Character, 128>> characters;
    };
} // namespace TempoUI
