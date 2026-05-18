#include "TextArea.h"
#include "Elements/BorderedBox/BorderedBox.h"
#include "Elements/Button/Button.h"
#include "Elements/Text/Text.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "TextHandler/TextHandler.h"
#include "ThemeManager/ThemeManager.h"
#include "EventManager/EventManager.h"

namespace TempoUI
{
    TextAreaConfig::TextAreaConfig()
    {
        if (!ThemeManager::get().is_loaded())
            return;

        if (JSONToken* textarea = ThemeManager::get().try_get("TextArea"))
        {
            for (const auto& [name, value] : textarea->as_object())
            {
                if (name == "Text Color")
                {
                    text_color_ = value.as_vec4();
                }
            }
        }
    }

    TextAreaConfig& TextAreaConfig::text_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::TextAreaValues::Text_Color;
        text_color_ = color;
        return *this;
    }

    TextArea::TextArea(const TextAreaConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex())
    {
        config.size = _config.size();
        config.localPosition = _config.position();
        config.state |= CLICKABLE;
        config.state |= HOVERABLE;

        current_color_ = _config.text_color();
        texts_.push_back(create_child<Text>(TextConfig()
                .id("Line")
                .text("")
                .position(glm::vec2(5.f, -TextHandler::get().get_global_decent("default_font")))
                .color(_config.text_color())));

        texts_.back()->set_should_wrap(false);
        lines_of_text_.emplace_back("");

        cursor_ = UIMesh(_config.position(), glm::vec2(2.f, TextHandler::get().get_global_ascent("default_font")), glm::vec4(0.f, 0.f, 0.f, 0.f));
    }

    TextArea::TextArea(const std::string& id, glm::vec2 pos, glm::vec2 size) : ElementBase(id)
    {
        config.size = size;
        config.localPosition = pos;
        config.state |= CLICKABLE;
        config.state |= HOVERABLE;

        texts_.push_back(create_child<Text>(TextConfig().id("Line").text("").position(glm::vec2(5.f, 5.f))));
        texts_.back()->set_should_wrap(false);
        lines_of_text_.emplace_back("");

        cursor_ = UIMesh(glm::vec2(pos), glm::vec2(2.f, TextHandler::get().get_global_ascent("default_font")), glm::vec4(0.f, 0.f, 0.f, 0.f));
    }

    void TextArea::self_on_char(char character)
    {
        if (is_focused_)
        {
            reset_cursor();

            raw_text_.insert(raw_text_.begin() + cursor_index_, character);
            cursor_index_++;
            recalculate_layout();
            mark_dirty();
        }
    }

    void TextArea::self_on_key(UI::Key key, UI::Action action)
    {
        if (!is_focused_ || action == UI::Action::Release)
            return;

        reset_cursor();

        switch (key)
        {
        case UI::Key::Enter:
        {
            raw_text_.insert(raw_text_.begin() + cursor_index_, '\n');
            cursor_index_++;
            recalculate_layout();
            mark_dirty();
        }
        break;
        case UI::Key::Backspace:
        {
            if (cursor_index_ == 0)
                break;
            cursor_index_--;
            raw_text_.erase(cursor_index_, 1);
            recalculate_layout();
            mark_dirty();
        }
        break;
        case UI::Key::Left:
            if (cursor_index_ == 0)
                break;
            cursor_index_--;
            mark_dirty();
            break;
        case UI::Key::Right:
            if (cursor_index_ >= raw_text_.length())
                break;
            cursor_index_++;
            mark_dirty();
            break;

        case UI::Key::Up:
        {
            if (line_lengths_.empty())
                break;

            if (cursor_index_ < line_lengths_[0] + 1)
            {
                cursor_index_ = 0;
                mark_dirty();
                break;
            }

            std::string current_line;
            int prev_line_index = 0;
            int current_line_index = 0;

            for (int i = 0; i < lines_of_text_.size(); i++)
            {
                const std::string str = lines_of_text_[i];
                const int line_length = (int)str.length();
                const int stored_length = line_lengths_[i];

                if (cursor_index_ < current_line_index + line_length + 1)
                    break;

                prev_line_index = current_line_index;
                current_line_index += line_length;
                if (line_length < stored_length)
                {
                    current_line_index++;
                }
                current_line = str;
            }
            int new_cursor_index = std::min(prev_line_index + (int)current_line.length(), prev_line_index + (int)cursor_index_ - current_line_index);
            cursor_index_ = std::max(0, new_cursor_index);
            mark_dirty();
        }
        break;
        case UI::Key::Down:
        {
            int current_line_index = 0;
            int index_last_line = 0;
            for (int i = 0; i < lines_of_text_.size(); i++)
            {
                const std::string str = lines_of_text_[i];
                const int line_length = (int)str.length();
                const int stored_length = line_lengths_[i];
                if (cursor_index_ < current_line_index)
                {
                    break;
                }

                index_last_line = current_line_index;
                current_line_index += line_length;

                if (line_length < stored_length)
                {
                    current_line_index++;
                }
            }
            cursor_index_ = std::min(current_line_index + (int)(cursor_index_ - index_last_line), (int)raw_text_.length());
            mark_dirty();
        }
        break;
        default:
            break;
        }
    }

    void TextArea::self_on_mouse_button(UI::MouseButton button, UI::Action action)
    {
        if (action != UI::Action::Press)
            return;

        if (button == UI::MouseButton::Left)
        {
            is_focused_ = config.state & IS_HOVERED;
            if (is_focused_)
            {
                recalculate_cursor();
                reset_cursor();
            }
            else
            {
                cursor_.update_color({ 0.f, 0.f, 0.f, 0.f });
                mark_dirty();
            }
        }
    }

    void TextArea::recalculate_cursor()
    {
        if (lines_of_text_.empty())
        {
            cursor_index_ = 0;
            return;
        }

        glm::dvec2 mouse_pos = EventManager::get().get_mouse_position();
        float global_ascent = TextHandler::get().get_global_ascent("default_font");
        int proportionate_line_index = (int)(((mouse_pos.y - get_position().y) / global_ascent));

        if (lines_of_text_.size() - 1 < proportionate_line_index)
        {
            cursor_index_ = raw_text_.size();
            mark_dirty();
            return;
        }
        int index = 0;
        float x_offset = 0.f;
        for (int i = 0; i < proportionate_line_index; i++)
        {
            index += line_lengths_[i];
        }

        const std::array<Character, 128>& characters = TextHandler::get().get_characters("default_font");

        for (char c : lines_of_text_[proportionate_line_index])
        {
            Character ch = characters[(unsigned char)c];
            if (x_offset + ch.advance > (mouse_pos.x - get_position().x + 5.f))
            {
                break;
            }
            x_offset += ch.advance;
            index++;
        }
        cursor_index_ = index;
        mark_dirty();
    }

    void TextArea::self_update(float dt)
    {
        if (!is_focused_)
            return;

        blink_timer_ += dt;
        if (blink_timer_ >= 0.4f)
        {
            blink_timer_ = 0.0f;
            float alpha = cursor_.get_color().w;
            cursor_.update_color({ 0.f, 0.f, 0.f, 1.f - alpha });
            mark_dirty();
        }
    }

    void TextArea::update_buffers()
    {
        vertices.clear();
        indices.clear();

        vertices.reserve(4);
        indices.reserve(6);

        glm::vec2 text_size = glm::vec2(0.f);
        cursor_.update_size(glm::vec2(2.f, TextHandler::get().get_global_ascent("default_font")) * global_scale_.y);

        int current_line_index = 0;
        int start_index = 0;
        int end_index = (int)cursor_index_;

        if (line_lengths_.empty())
        {
            current_line_index = 0;
        }
        else
        {
            for (int i = 0; i < lines_of_text_.size(); i++)
            {
                const std::string str = lines_of_text_[i];
                const size_t stored_length = line_lengths_[i];
                const size_t text_length = str.length();

                if (start_index + text_length < end_index)
                {
                    current_line_index = i;
                    start_index += (int)text_length;
                    if (text_length < stored_length)
                    {
                        start_index++;
                    }
                }
                else
                {
                    current_line_index = i;
                    break;
                }
            }
        }

        if (!lines_of_text_.empty())
        {
            std::string sub = raw_text_.substr(start_index, end_index - start_index);
            text_size = TextHandler::get().calculate_size_of_text("default_font", sub);
        }

        cursor_.update_position(get_position() + glm::vec2(5.f + text_size.x * global_scale_.y, 5 + (static_cast<float>(current_line_index) * TextHandler::get().get_global_ascent("default_font") * global_scale_.y)));

        for (const unsigned int& index : cursor_.get_indices())
        {
            indices.push_back(index + vertices.size());
        }
        vertices.insert(vertices.end(), cursor_.get_vertices().begin(), cursor_.get_vertices().end());
    }

    void TextArea::self_theme_updated()
    {
        TextAreaConfig tbc;
        if (!(custom_values & CustomFlags::TextAreaValues::Text_Color))
        {
            for (Text* text : texts_)
            {
                text->update_color(tbc.text_color());
                current_color_ = tbc.text_color();
            }
        }
    }

    void TextArea::reset_cursor()
    {
        cursor_.update_color({ 0.f, 0.f, 0.f, 1.f });
        blink_timer_ = 0.f;
        mark_dirty();
    }

    void TextArea::recalculate_layout()
    {
        lines_of_text_.clear();
        line_lengths_.clear();

        lines_of_text_.reserve(1000);
        lines_of_text_.emplace_back("");
        line_lengths_.emplace_back(0);

        int current_line = 0;
        for (const char& c : raw_text_)
        {
            if (c != '\n')
            {
                if (config.size.x < TextHandler::get().calculate_size_of_text("default_font", lines_of_text_[current_line] + c).x * global_scale_.y)
                {
                    size_t last_space = lines_of_text_[current_line].rfind(' ');
                    std::string new_string;
                    if (last_space != std::string::npos)
                    {
                        new_string = lines_of_text_[current_line].substr(last_space + 1);
                        lines_of_text_[current_line].erase(last_space);
                        line_lengths_[current_line] = (int)lines_of_text_[current_line].length() + 1;
                    }
                    current_line++;
                    lines_of_text_.emplace_back(new_string);
                    line_lengths_.emplace_back(new_string.length());
                }
                lines_of_text_[current_line] += c;
                line_lengths_[current_line]++;
            }
            else
            {
                line_lengths_[current_line]++;
                current_line++;
                lines_of_text_.emplace_back("");
                line_lengths_.emplace_back(0);
            }
        }
        update_texts();
    }

    void TextArea::update_texts()
    {
        const int text_size = (int)texts_.size();
        const int lines_of_text = (int)lines_of_text_.size();
        const float global_ascent = TextHandler::get().get_global_ascent("default_font");
        const float global_descent = TextHandler::get().get_global_decent("default_font");

        if (texts_.size() != lines_of_text_.size())
        {
            if (texts_.size() > lines_of_text_.size())
            {
                while (texts_.size() > lines_of_text_.size())
                {
                    remove_element(texts_.back()->ID);
                    texts_.pop_back();
                }
            }
            else
            {
                for (int i = text_size; i < lines_of_text; i++)
                {
                    text_child_indexes_.push_back((int)texts_.size());
                    texts_.push_back(create_child<Text>(std::format("Line{}", i), "", glm::vec2(5.f, -((global_descent) + (float)i * global_ascent) * global_scale_.y), current_color_));
                    texts_.back()->set_should_wrap(false);
                }
            }
        }

        for (int i = 0; i < texts_.size(); i++)
        {
            texts_[i]->set_local_position(glm::vec2(5.f * global_scale_.y, (-global_descent + (float)i * global_ascent) * global_scale_.y));
            texts_[i]->update_text(lines_of_text_[i]);
            texts_[i]->scale_updated(global_scale_);
        }
    }

    void TextArea::self_scale_updated(glm::vec2 new_scale)
    {
        recalculate_layout();
    }

    std::string TextArea::get_text()
    {
        return raw_text_;
    }

    static LayoutRegistry::AutoRegister dummy("TextArea", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
    {
        TextAreaConfig config;
        config.id(props.Id.value());
        if (props.Pos)
            config.position(props.Pos.value());
        if (props.Size)
            config.size(props.Size.value());
        if (props.Flex)
            config.flex(props.Flex.value());

        if (auto it = json_object.find("Text Color"); it != json_object.end())
        {
            config.text_color(it->second.as_vec4());
        }

        return std::make_unique<TextArea>(config);
    });
} // namespace TempoUI
