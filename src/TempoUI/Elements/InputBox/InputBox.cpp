#include "InputBox.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "TextHandler/TextHandler.h"
#include "ThemeManager/ThemeManager.h"

namespace TempoUI
{
    InputBoxConfig ::InputBoxConfig()
    {
        if (!ThemeManager::get().is_loaded())
        {
            return;
        }
        if (JSONToken* token = ThemeManager::get().try_get("InputBox"))
        {
            const std::map<std::string, JSONToken>& mapped_values = token->as_object();
            for (const auto& [name, value] : mapped_values)
            {
                if (name == "Background Color")
                {
                    background_color_ = value.as_vec4();
                }
                else if (name == "Cursor Color")
                {
                    cursor_color_ = value.as_vec4();
                }
                else if (name == "Text Color")
                {
                    text_color_ = value.as_vec4();
                }
                else if (name == "Radius")
                {
                    radius_ = value.as_vec4();
                }
            }
        }
    }
    InputBoxConfig& InputBoxConfig::background_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::InputBoxValues::Background_Color;
        background_color_ = color;
        return *this;
    }
    InputBoxConfig& InputBoxConfig::cursor_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::InputBoxValues::Cursor_Color;
        cursor_color_ = color;
        return *this;
    }
    InputBoxConfig& InputBoxConfig::text_color(glm::vec4 color)
    {
        custom_values |= CustomFlags::InputBoxValues::Text_Color;
        text_color_ = color;
        return *this;
    }

    InputBox::InputBox(const InputBoxConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex())
    {
        config.size = _config.size();
        config.localPosition = _config.position();
        config.state |= CLICKABLE;
        config.state |= HOVERABLE;
        config.radius = _config.radius();

        background_color_ = _config.background_color();
        text_color_ = _config.text_color();
        cursor_color_ = _config.cursor_color();

        meshes_[0] = UIMesh(get_position(), _config.size(), _config.background_color(), config.radius);
        float y_offset = _config.size().y * 0.1f;
        meshes_[1] = UIMesh(get_position() + glm::vec2(0.f, y_offset), { 2.f, _config.size().y - (y_offset * 2.f) }, glm::vec4(glm::vec3(cursor_color_), 0.0f));
        text_ = create_child<Text>(TextConfig()
                .id("")
                .text("")
                .position({ 5.f, 0.f })
                .color(_config.text_color())
                .align_vert(TextVerticalAlignment::Center));
        text_->set_should_wrap(false);
    }

    InputBox::InputBox(const std::string& id, glm::vec2 pos, glm::vec2 size) : ElementBase(id)
    {
        config.size = size;
        config.localPosition = pos;
        config.state |= CLICKABLE;
        config.state |= HOVERABLE;

        meshes_[0] = UIMesh(get_position(), size, glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
        float y_offset = size.y * 0.1f;
        meshes_[1] = UIMesh(get_position() + glm::vec2(0.f, y_offset), { 2.f, TextHandler::get().get_global_ascent("default_font") }, glm::vec4(0.7f, 0.7f, 0.7f, 0.0f));
        text_ = create_child<Text>("", "", glm::vec2(5.f, 5.f), glm::vec3(1.f));
        text_->set_should_wrap(false);
    }

    InputBox::~InputBox()
    {
    }

    std::string InputBox::get_text()
    {
        return text_ != nullptr ? text_->get_text() : "";
    }

    void InputBox::on_enter(std::function<void(std::string)> func)
    {
        callback_ = std::make_unique<std::function<void(std::string)>>(std::move(func));
    }

    void InputBox::self_on_mouse_button(UI::MouseButton button, UI::Action action)
    {
        if (action == UI::Action::Press)
        {
            switch (button)
            {
            case UI::MouseButton::Left:
                if (config.state & IS_HOVERED)
                {
                    is_focused_ = true;
                    meshes_[1].update_color({ glm::vec3(cursor_color_), 1.0 });
                    blink_timer_ = 0.f;
                    mark_dirty();
                }
                else
                {
                    is_focused_ = false;
                    meshes_[1].update_color({ glm::vec3(cursor_color_), 0.0 });
                    mark_dirty();
                }
                break;
            default:
                break;
            }
        }
    }

    void InputBox::self_on_char(char character)
    {
        if (!is_focused_ || !is_allowed(character))
            return;

        reset_cursor();

        std::string text = text_->get_text();
        text.insert(cursor_index_, 1, character);
        cursor_index_++;
        text_->update_text(text);
        mark_dirty();
    }

    void InputBox::self_on_key(UI::Key key, UI::Action action)
    {
        if (!is_focused_)
            return;

        if (action == UI::Action::Press || action == UI::Action::Repeat)
        {
            switch (key)
            {
            case UI::Key::Backspace:
            {
                std::string current_text = text_->get_text();
                if (current_text.empty() || cursor_index_ == 0)
                    break;

                cursor_index_--;
                current_text.erase(cursor_index_, 1);
                text_->update_text(current_text);
                reset_cursor();
                mark_dirty();
            }
            break;
            case UI::Key::Left:
                if (cursor_index_ > 0)
                {
                    cursor_index_--;
                    reset_cursor();
                    mark_dirty();
                }
                break;
            case UI::Key::Right:
                if (cursor_index_ < text_->get_text().length())
                {
                    cursor_index_++;
                    reset_cursor();
                    mark_dirty();
                }
                break;
            case UI::Key::Enter:
                if (callback_)
                {
                    (*callback_)(get_text());
                }
                break;
            default:
                break;
            }
        }
    }

    void InputBox::self_update(float dt)
    {
        if (!is_focused_)
            return;

        blink_timer_ += dt;
        if (blink_timer_ >= 0.4f)
        {
            blink_timer_ = 0.0f;
            float alpha = meshes_[1].get_color().w;
            meshes_[1].update_color({ glm::vec3(cursor_color_), 1.f - alpha });
            mark_dirty();
        }
    }

    void InputBox::self_theme_updated()
    {
        InputBoxConfig ibc;
        if (!(custom_values & CustomFlags::InputBoxValues::Background_Color))
        {
            meshes_[0].update_color(ibc.background_color());
        }
        if (!(custom_values & CustomFlags::InputBoxValues::Cursor_Color))
        {
            meshes_[1].update_color(ibc.cursor_color());
        }
        if (!(custom_values & CustomFlags::InputBoxValues::Text_Color))
        {
            text_->update_color(ibc.text_color());
        }
        if (!(custom_values & CustomFlags::Radius))
        {
            meshes_[0].update_radius(ibc.radius());
        }
        mark_dirty();
    }

    void InputBox::update_buffers()
    {
        if (text_ != nullptr)
        {
            std::string current_text = text_->get_text();
            float cursor_x_offset = 0.0f;
            for (int i = 0; i < cursor_index_; i++)
            {
                cursor_x_offset += TextHandler::get().get_character("default_font", current_text[i]).advance;
            }

            float unscaled_total_width = 0.0f;
            for (char c : current_text)
            {
                unscaled_total_width += TextHandler::get().get_character("default_font", c).advance;
            }

            float text_scale = 1.0f;
            if (unscaled_total_width > 0.f && text_->get_size().x > 0.f)
            {
                text_scale = std::min(1.0f, text_->get_size().x / unscaled_total_width);
            }

            cursor_x_offset *= (text_scale * global_scale_.y);

            float text_pos_x = text_->get_local_position().x;

            float relative_cursor_x = text_pos_x + cursor_x_offset;

            if (relative_cursor_x > config.size.x - 5.f)
            {
                text_pos_x -= (relative_cursor_x - (config.size.x - 5.f));
            }
            else if (relative_cursor_x < 5.f)
            {
                text_pos_x += (5.f - relative_cursor_x);
            }

            text_->set_local_position(glm::vec2(text_pos_x, text_->get_local_position().y));
            text_->update_absolute_position();

            meshes_[0].update_position(get_position());
            meshes_[0].update_size(config.size);

            glm::vec2 updated_text_pos = text_->get_position();
            meshes_[1].update_position({ updated_text_pos.x + cursor_x_offset, get_position().y + (config.size.y * 0.1f) });

            float y_offset = config.size.y * 0.1f;
            meshes_[1].update_size({ 2.f * global_scale_.x, config.size.y - (y_offset * 2.f) });
        }

        vertices.clear();
        indices.clear();

        vertices.reserve(meshes_.size() * 4);
        indices.reserve(meshes_.size() * 6);

        for (UIMesh& mesh : meshes_)
        {
            std::array<Vertex, 4> mesh_vertices = mesh.get_vertices();
            std::array<unsigned int, 6> mesh_indices = mesh.get_indices();

            for (const unsigned int& index : mesh_indices)
            {
                indices.emplace_back(index + vertices.size());
            }
            vertices.insert(vertices.end(), mesh_vertices.begin(), mesh_vertices.end());
        }

        for (const auto& child : children)
        {
            child->update_absolute_position();
            child->update_buffers();
        }
    }

    bool InputBox::is_allowed(char c) const
    {
        return !(chars_.test(static_cast<unsigned char>(c)));
    }

    void InputBox::reset_cursor()
    {
        blink_timer_ = 0.f;
        meshes_[1].update_color(cursor_color_);
    }

    static LayoutRegistry::AutoRegister dummy("InputBox", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
    {
        InputBoxConfig config;
        config.id(props.Id.value());
        if (props.Pos)
            config.position(props.Pos.value());
        if (props.Size)
            config.size(props.Size.value());
        if (props.Radius)
            config.radius(props.Radius.value());
        if (props.Flex)
            config.flex(props.Flex.value());

        if (auto it = json_object.find("Cursor Color"); it != json_object.end())
        {
            config.cursor_color(it->second.as_vec4());
        }
        if (auto it = json_object.find("Background Color"); it != json_object.end())
        {
            config.background_color(it->second.as_vec4());
        }
        if (auto it = json_object.find("Text Color"); it != json_object.end())
        {
            config.text_color(it->second.as_vec4());
        }

        return std::make_unique<InputBox>(config);
    });
} // namespace TempoUI
