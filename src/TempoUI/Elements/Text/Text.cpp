#include "Text.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "TextHandler/TextHandler.h"
#include "UIMesh/UIMesh.h"
#include "ThemeManager/ThemeManager.h"

namespace TempoUI
{
    Text::Text(const TextConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex()),
                                            color_(_config.color()), text_(_config.text()), vert_alignment_(_config.align_vert()), hori_alignment_(_config.align_hori())
    {
        config.localPosition = _config.position();
        config.size = TextHandler::get().calculate_size_of_text("default_font", text_);
    }

    Text::Text(const std::string& id, const std::string& text, glm::vec2 pos, glm::vec3 color) : ElementBase(id), color_(color), text_(text)
    {
        config.localPosition = pos;
        config.size = TextHandler::get().calculate_size_of_text("default_font", text);
    }

    void Text::update_text(const std::string& text)
    {
        glm::vec2 parent_size = (parent) ? parent->get_size() : glm::vec2(0.f);

        if (written_text == text && parent_size == last_parent_size_ && current_scale == prev_scale)
            return;

        last_parent_size_ = parent_size;
        prev_scale = current_scale;

        meshes_.clear();
        meshes_.reserve(text.size());

        const std::array<Character, 128>& characters = TextHandler::get().get_characters("default_font");
        float global_ascent = TextHandler::get().get_global_ascent("default_font");
        float max_allowed_width = config.size.x;

        max_allowed_width = std::max(0.f, parent_size.x - (config.localPosition.x * 2.f));
        float max_height = (parent) ? parent_size.y : config.size.y;

        float scale = 1.f;
        if (max_height > 0.f && global_ascent > max_height)
        {
            scale = max_height / global_ascent;
        }

        float cap_height = static_cast<float>(characters.at('H').Bearing.y);

        float temp_x = 0.f;
        float temp_y = cap_height * (scale * current_scale.y);
        float max_x = 0.f;

        int last_space_index = -1;
        int last_space_char_index = -1;

        for (auto [str_index, c] : std::views::enumerate(text))
        {
            if (c == '\n')
            {
                temp_y += global_ascent * (scale * current_scale.y);
                temp_x = 0.f;
                last_space_index = -1;
                continue;
            }

            const Character ch = characters.at(c);

            if (c == ' ')
            {
                last_space_index = (int)meshes_.size();
                last_space_char_index = (int)str_index;
            }

            if (temp_x + (ch.advance * scale) > max_allowed_width && should_wrap_)
            {
                if (last_space_index != -1)
                {
                    float x_offset = 0.f;
                    int current_char_idx = last_space_char_index + 1;

                    for (int i = last_space_index + 1; i < meshes_.size(); i++)
                    {
                        const Character ch_inner = characters.at(text[current_char_idx]);
                        glm::vec2 mesh_pos = meshes_[i].get_position();
                        meshes_[i].update_position(glm::vec2(get_local_position().x + x_offset, get_local_position().y + (global_ascent * (scale * current_scale.y))));
                        x_offset += ch_inner.advance * (scale * current_scale.y);
                        current_char_idx++;
                    }
                    temp_x = x_offset;
                    temp_y += global_ascent * (scale * current_scale.y);
                    last_space_index = -1;
                }
                else
                {
                    temp_x = 0.f;
                    temp_y += global_ascent * (scale * current_scale.y);
                }
            }

            const float x_pos = temp_x + ((float)ch.Bearing.x * (scale * current_scale.y));
            const float y_pos = temp_y - ((float)ch.Bearing.y * (scale * current_scale.y));
            const float width = static_cast<float>(ch.Size.x) * (scale * current_scale.y);
            const float height = static_cast<float>(ch.Size.y) * (scale * current_scale.y);

            meshes_.emplace_back(glm::vec2(x_pos, y_pos), glm::vec2(width, height), glm::vec4(color_, 1.f), glm::vec4(0.f), 1);
            meshes_.back().update_texture_coords(ch.uvOffset, ch.uvOffset + ch.uvSize);

            temp_x += ch.advance * (scale * current_scale.y);
            max_x = glm::max(max_x, temp_x);
        }

        float v_offset = 0.f;
        float h_offset = 0.f;
        switch (vert_alignment_)
        {
        case TextVerticalAlignment::Top:
            break;
        case TextVerticalAlignment::Center:
            v_offset = (max_height - temp_y) * 0.5f;
            break;
        case TextVerticalAlignment::Bottom:
            break;
        }
        switch (hori_alignment_)
        {
        case TextHorizontalAlignment::Left:
            break;
        case TextHorizontalAlignment::Center:
            h_offset = ((max_allowed_width) * 0.5f) - (max_x * 0.5f);
            break;
        case TextHorizontalAlignment::Right:
            h_offset = (max_allowed_width - max_x);
            break;
        }

        for (UIMesh& mesh : meshes_)
        {
            glm::vec2 p = mesh.get_position();

            if (h_offset > 0.f)
            {
                p.x += h_offset;
            }

            if (v_offset > 0.0f)
            {
                p.y += v_offset;
            }
            mesh.update_position(p);
        }

        config.size = glm::vec2(max_x, temp_y + ((scale * global_ascent * current_scale.y) * 0.5f));

        this->text_ = text;
        this->written_text = text;

        mark_dirty();
    }

    void Text::update_color(glm::vec4 color)
    {
        color_ = color;
        for (UIMesh& mesh : meshes_)
        {
            mesh.update_color(color);
        }
        mark_dirty();
    }

    void Text::set_should_wrap(bool should_wrap)
    {
        should_wrap_ = should_wrap;
    }

    Text::~Text()
    {
    }

    void Text::update_buffers()
    {
        update_text(text_);

        vertices.clear();
        indices.clear();

        vertices.reserve(meshes_.size() * 4);
        indices.reserve(meshes_.size() * 6);

        const glm::vec2 pos = get_position();

        for (UIMesh& mesh : meshes_)
        {
            for (const unsigned int index : mesh.get_indices())
            {
                indices.push_back(index + vertices.size());
            }

            for (const Vertex& vertex : mesh.get_vertices())
            {
                Vertex shifted_vertex = vertex;
                shifted_vertex.position += pos;
                vertices.push_back(shifted_vertex);
            }
        }
    }

    void Text::self_theme_updated()
    {
        TextConfig tc;
        if (!(custom_values & CustomFlags::TextValues::Color))
        {
            color_ = tc.color();
            for (UIMesh& mesh : meshes_)
            {
                mesh.update_color(glm::vec4(color_, 1.f));
            }
        }
    }

    void Text::self_scale_updated(glm::vec2 new_scale)
    {
        current_scale = new_scale;
    }

    TextConfig::TextConfig()
    {
        if (!ThemeManager::get().is_loaded())
            return;
        if (JSONToken* text = ThemeManager::get().try_get("Text"))
        {
            const std::map<std::string, JSONToken>& mapped_values = text->as_object();
            for (const auto& [name, value] : mapped_values)
            {
                if (name == "Color")
                {
                    color_ = value.as_vec3();
                }
            }
        }
    }

    TextConfig& TextConfig::text(std::string text)
    {
        custom_values |= CustomFlags::TextValues::Text;
        text_ = std::move(text);
        return *this;
    }

    TextConfig& TextConfig::color(glm::vec3 color)
    {
        custom_values |= CustomFlags::TextValues::Color;
        color_ = color;
        return *this;
    }

    TextConfig& TextConfig::align_vert(TextVerticalAlignment alignment)
    {
        custom_values |= CustomFlags::TextValues::TextVertAlignment;
        vert_alignment_ = alignment;
        return *this;
    }

    TextConfig& TextConfig::align_hori(TextHorizontalAlignment alignment)
    {
        custom_values |= CustomFlags::TextValues::TextHoriAlignment;
        hori_alignment_ = alignment;
        return *this;
    }

    static LayoutRegistry::AutoRegister dummy("Text", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
    {
        TextConfig config;
        if (props.Id)
            config.id(props.Id.value());
        if (props.Pos)
            config.position(props.Pos.value());
        if (props.Size)
            config.size(props.Size.value());
        if (props.Radius)
            config.radius(props.Radius.value());
        if (props.Flex)
            config.flex(props.Flex.value());

        if (auto it = json_object.find("Text"); it != json_object.end())
        {
            config.text(it->second.as_string());
        }
        if (auto it = json_object.find("Color"); it != json_object.end())
        {
            config.color(it->second.as_vec3());
        }
        if (auto it = json_object.find("VAlign"); it != json_object.end())
        {
            std::string alignment = it->second.as_string();
            if (alignment == "Top")
            {
                config.align_vert(TextVerticalAlignment::Top);
            }
            else if (alignment == "Center")
            {
                config.align_vert(TextVerticalAlignment::Center);
            }
            else if (alignment == "Bottom")
            {
                config.align_vert(TextVerticalAlignment::Bottom);
            }
        }
        if (auto it = json_object.find("HAlign"); it != json_object.end())
        {
            std::string alignment = it->second.as_string();
            if (alignment == "Left")
            {
                config.align_hori(TextHorizontalAlignment::Left);
            }
            else if (alignment == "Center")
            {
                config.align_hori(TextHorizontalAlignment::Center);
            }
            else if (alignment == "Right")
            {
                config.align_hori(TextHorizontalAlignment::Right);
            }
        }

        return std::make_unique<Text>(config);
    });
} // namespace TempoUI
