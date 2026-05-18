#include "TextHandler.h"
#include <ft2build.h>
#include FT_FREETYPE_H

namespace TempoUI
{
    TextHandler& TextHandler::get()
    {
        static TextHandler instance;
        return instance;
    }

    TextHandler::~TextHandler()
    {
    }

    uint32_t TextHandler::load_font(const std::string& id, const std::string& path, unsigned int font_size)
    {
        FT_Library lib = nullptr;
        if (FT_Init_FreeType(&lib))
        {
            std::cerr << "FT_Init_FreeType() failed" << '\n';
        }
        FT_Face face = nullptr;
        if (FT_New_Face(lib, path.c_str(), 0, &face))
        {
            std::cerr << "FT_New_Face() failed" << '\n';
        }

        FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(font_size));

        return load_chars_into_atlas(id, lib, face);
    }

    uint32_t TextHandler::load_font_from_memory(const std::string& id, const unsigned char* data, long length, unsigned int font_size)
    {
        FT_Library lib = nullptr;
        if (FT_Init_FreeType(&lib))
        {
            std::cerr << "FT_Init_FreeType() failed" << '\n';
        }
        FT_Face face = nullptr;
        if (FT_New_Memory_Face(lib, data, length, 0, &face))
        {
            std::cerr << "FT_New_Face() failed" << '\n';
        }

        FT_Set_Pixel_Sizes(face, 0, static_cast<FT_UInt>(font_size));

        return load_chars_into_atlas(id, lib, face);
    }

    const std::array<Character, 128>& TextHandler::get_characters(const std::string& id)
    {
        if (!characters.contains(id))
        {
            std::cout << "Character Set with ID: " << id << " Not Found!" << '\n';
        }
        return characters.at(id);
    }

    Character TextHandler::get_character(const std::string& id, char c)
    {
        return characters[id][c];
    }

    glm::vec2 TextHandler::calculate_size_of_text(const std::string& id, const std::string& text)
    {
        glm::vec2 size(0.f, 0.f);
        float total_width = 0.f;
        float max_height = FLT_MIN;
        float min_height = FLT_MAX;
        for (char c : text)
        {
            const Character ch = characters[id][c];
            total_width += ch.advance;
            max_height = std::max(max_height, static_cast<float>(ch.Bearing.y));
            min_height = std::min(min_height, static_cast<float>(ch.Bearing.y - ch.Size.y));
        }

        return { total_width, max_height + min_height };
    }

    float TextHandler::get_global_ascent(const std::string& id)
    {
        if (!global_ascents.contains(id))
        {
            std::cout << "Global Ascent not found for ID: " << id << '\n';
            return 0.f;
        }

        return global_ascents.at(id);
    }

    float TextHandler::get_global_decent(const std::string& id)
    {
        if (!global_descents.contains(id))
        {
            std::cout << "Global Descent not found for ID: " << id << '\n';
            return 0.f;
        }

        return global_descents.at(id);
    }

    uint32_t TextHandler::get_font(const std::string& id)
    {
        auto it = textures.find(id);
        if (it != textures.end())
            return it->second;
        return UINT32_MAX;
    }

#ifdef OPENGL
    uint32_t TextHandler::load_chars_into_atlas(const std::string& id, FT_Library& lib, FT_Face& face)
    {
        unsigned int atlas_width = 0;
        unsigned int atlas_height = 0;

        for (unsigned char c = 0; c < 128; ++c)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                continue;
            atlas_width += face->glyph->bitmap.width + 4;
            atlas_height = std::max(atlas_height, face->glyph->bitmap.rows);
        }
        atlas_height += 2;

        uint32_t atlas = UINT32_MAX;

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        const std::vector<unsigned char> empty_data(static_cast<size_t>(atlas_width * atlas_height), 0);

        glGenTextures(1, &atlas);
        glBindTexture(GL_TEXTURE_2D, atlas);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<GLsizei>(atlas_width), static_cast<GLsizei>(atlas_height),
            0, GL_RED, GL_UNSIGNED_BYTE, empty_data.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int x_offset = 0;

        for (unsigned char c = 0; c < 128; c++)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cerr << "ERROR::FREETYPE | FAILED TO LOAD GLYPH" << '\n';
                continue;
            }

            glTexSubImage2D(GL_TEXTURE_2D, 0, x_offset, 1, static_cast<GLsizei>(face->glyph->bitmap.width),
                static_cast<GLsizei>(face->glyph->bitmap.rows), GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            Character character = {
                glm::vec2(static_cast<float>(x_offset) / static_cast<float>(atlas_width), 0.f),
                glm::vec2(static_cast<float>(face->glyph->bitmap.width) / static_cast<float>(atlas_width), (static_cast<float>(face->glyph->bitmap.rows) + 1.f) / static_cast<float>(atlas_height)),
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<float>(face->glyph->advance.x >> 6)
            };
            characters[id][c] = character;

            x_offset += static_cast<int>(face->glyph->bitmap.width + 4);
        }

        global_ascents[id] = static_cast<float>(face->size->metrics.ascender >> 6);
        global_descents[id] = static_cast<float>(face->size->metrics.descender >> 6);

        textures[id] = atlas;

        glBindTexture(GL_TEXTURE_2D, 0);
        FT_Done_Face(face);
        FT_Done_FreeType(lib);

        return atlas;
    }
#else
    uint32_t TextHandler::load_chars_into_atlas(const std::string& id, FT_Library& lib, FT_Face& face) {}
#endif
} // namespace TempoUI
