#include "Combobox.h"
#include "Elements/Button/Button.h"
#include "Elements/ScrollBox/ScrollBox.h"
#include "Elements/Text/Text.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include "ThemeManager/ThemeManager.h"
#include "UIMesh/UIMesh.h" // IWYU pragma: keep

ComboboxConfig& ComboboxConfig::spacing(float spacing)
{
    custom_values |= CustomFlags::ComboboxValues::Spacing;
    spacing_ = spacing;
    return *this;
}
ComboboxConfig& ComboboxConfig::main_color(glm::vec4 color)
{
    custom_values |= CustomFlags::ComboboxValues::Main_Color;
    main_color_ = color;
    return *this;
}
ComboboxConfig& ComboboxConfig::sub_color(glm::vec4 color)
{
    custom_values |= CustomFlags::ComboboxValues::Sub_Color;
    sub_color_ = color;
    return *this;
}

Combobox::Combobox(const ComboboxConfig& _config) : ElementBase(_config.id(), _config.get_flags(), _config.flex()), spacing_(_config.spacing())
{
    config.size = _config.size();
    config.localPosition = _config.position();
    config.state |= HOVERABLE;
    config.state |= CLICKABLE;
    sub_color_ = _config.sub_color();

    button = create_child<Button>("Button" + _config.id(), glm::vec2(0.f), _config.size(), _config.main_color());
    main_text_ = button->create_child<Text>(TextConfig()
            .id("Main_Text" + _config.id())
            .text("...")
            .position({ 5.f, 0.f })
            .align_hori(TextHorizontalAlignment::Center)
            .align_vert(TextVerticalAlignment::Center));
    scroll_box = create_child<ScrollBox>("Scroll_Box" + _config.id(), glm::vec2(0.f, _config.size().y + _config.spacing()), glm::vec2(_config.size().x, (_config.size().y + _config.spacing()) * 4.f), _config.spacing(), Direction::Vertical);
    scroll_box->set_state(VISIBLE, false);
    scroll_box->set_state(IS_CONTAINER, false);
    scroll_box->set_state(IS_OVERLAY, true);

    button->set_on_click([this]()
    {
        expanded = !expanded;
        scroll_box->set_state(VISIBLE, expanded);
    });
}

Combobox::Combobox(const std::string& id, glm::vec2 pos, glm::vec2 size, float spacing) : ElementBase(id), spacing_(spacing)
{
    config.size = size;
    config.localPosition = pos;
    config.state |= HOVERABLE;
    config.state |= CLICKABLE;

    button = create_child<Button>("Button" + id, glm::vec2(0.f), size, glm::vec4(0.8f, 0.8f, 0.8f, 1.f));
    main_text_ = button->create_child<Text>(TextConfig()
            .id("Main_Text" + id)
            .text("...")
            .position({ 5.f, 0.f })
            .align_hori(TextHorizontalAlignment::Center)
            .align_vert(TextVerticalAlignment::Center));

    scroll_box = create_child<ScrollBox>("Scroll_Box" + id, glm::vec2(0.f, size.y + spacing), glm::vec2(size.x, (size.y + spacing) * 4.f), spacing, Direction::Vertical);
    scroll_box->set_state(VISIBLE, false);
    scroll_box->set_state(IS_OVERLAY, true);

    button->set_on_click([this]()
    {
        expanded = !expanded;
        scroll_box->set_state(VISIBLE, expanded);
    });
}

void Combobox::add_option(const std::string& option)
{
    if (scroll_box->get_children().empty())
    {
        main_text_->update_text(option);
    }

    Button* temp_button = scroll_box->create_child<Button>("SB_" + option, glm::vec2(0.f), config.size, sub_color_);
    Text* text = temp_button->create_child<Text>(TextConfig()
            .id("SBT_" + option)
            .text(option)
            .position({ 5.f, 0.f })
            .align_hori(TextHorizontalAlignment::Center)
            .align_vert(TextVerticalAlignment::Center));

    text->set_should_wrap(false);

    int index = (int)scroll_box->get_children().size();

    temp_button->set_on_click([this, option, index]()
    {
        main_text_->update_text(option);
        button->trigger_on_click();
        selected_index = index;
    });
}

void Combobox::add_multiple_options(std::initializer_list<std::string> options)
{
    int index = (int)scroll_box->get_children().size();
    if (scroll_box->get_children().empty())
    {
        main_text_->update_text(*options.begin());
    }

    for (const std::string& str : options)
    {
        Button* temp_button = scroll_box->create_child<Button>("SB_" + str, glm::vec2(0.f), config.size, sub_color_);
        Text* text = temp_button->create_child<Text>("SBT_" + str, str, glm::vec2(5.f, 0.f));
        text->set_should_wrap(false);

        temp_button->set_on_click([this, str, index]()
        {
            selected_index = index;
            main_text_->update_text(str);
            button->trigger_on_click();
        });
        index++;
    }
}

int Combobox::get_value_as_int()
{
    return std::stoi(main_text_->get_text());
}

std::string Combobox::get_value()
{
    return main_text_->get_text();
}

bool Combobox::isHovered()
{
    if (expanded && scroll_box->isHovered())
    {
        return true;
    }
    return ElementBase::isHovered();
}

void Combobox::self_on_scroll(float y_offset)
{
    if (!button->check_state(IS_HOVERED))
        return;

    const std::vector<std::unique_ptr<ElementBase>>& elements = scroll_box->get_children();
    if (elements.empty())
        return;

    selected_index = std::clamp(selected_index - (int)y_offset, 0, (int)elements.size() - 1);

    const auto& btn_children = elements[selected_index]->get_children();
    if (!btn_children.empty())
    {
        if (Text* gotten_text = dynamic_cast<Text*>(btn_children[0].get()))
        {
            main_text_->update_text(gotten_text->get_text());
        }
    }
}

void Combobox::self_on_mouse_button(UI::MouseButton button, UI::Action action)
{
    if (action == UI::Action::Press && expanded && !isHovered())
    {
        this->button->trigger_on_click();
    }
}

void Combobox::self_theme_updated()
{
    ComboboxConfig _config; // NOLINT
    if (!(custom_values & CustomFlags::ComboboxValues::Main_Color))
    {
        button->update_color(_config.main_color());
    }
    if (!(custom_values & CustomFlags::ComboboxValues::Sub_Color))
    {
        sub_color_ = _config.sub_color();
        for (auto& child : scroll_box->get_children())
        {
            if (Button* child_button = dynamic_cast<Button*>(child.get()))
            {
                child_button->update_color(sub_color_);
            }
        }
    }
    if (!(custom_values & CustomFlags::Radius))
    {
        config.radius = _config.radius();
        button->update_radius(config.radius);
        for (auto& child : scroll_box->get_children())
        {
            if (Button* child_button = dynamic_cast<Button*>(child.get()))
            {
                child_button->update_radius(config.radius);
            }
        }
    }
}

void Combobox::update_buffers()
{
    if (button)
        button->set_size(config.size);

    if (scroll_box)
    {
        scroll_box->set_local_position(glm::vec2(0.f, config.size.y + spacing_));
        glm::vec2 sb_size = scroll_box->get_size();
        scroll_box->set_size({ config.size.x, sb_size.y });
    }

    for (auto& child : children)
    {
        child->update_absolute_position();
        child->update_buffers();
    }
}

ComboboxConfig::ComboboxConfig()
{
    if (!ThemeManager::get().is_loaded())
        return;

    if (JSONToken* token = ThemeManager::get().try_get("Combobox"))
    {
        const std::map<std::string, JSONToken>& mapped_values = token->as_object();
        for (const auto& [name, value] : mapped_values)
        {
            if (name == "Main Color")
            {
                main_color_ = value.as_vec4();
            }
            else if (name == "Sub Color")
            {
                sub_color_ = value.as_vec4();
            }
            else if (name == "Radius")
            {
                radius_ = value.as_vec4();
            }
        }
    }
}

static LayoutRegistry::AutoRegister dummy("Combobox", [](const JSON_OBJECT& json_object, const BaseProperties& props) // NOLINT
{
    ComboboxConfig config;
    config.id(props.Id.value());
    if (props.Pos)
        config.position(props.Pos.value());
    if (props.Size)
        config.size(props.Size.value());
    if (props.Radius)
        config.radius(props.Radius.value());
    if (props.Flex)
        config.flex(props.Flex.value());

    if (auto it = json_object.find("Main Color"); it != json_object.end())
    {
        config.main_color(it->second.as_vec4());
    }
    if (auto it = json_object.find("Sub Color"); it != json_object.end())
    {
        config.sub_color(it->second.as_vec4());
    }
    if (auto it = json_object.find("Spacing"); it != json_object.end())
    {
        config.spacing((float)it->second.as_number());
    }
    std::unique_ptr<Combobox> combo = std::make_unique<Combobox>(config);
    if (auto it = json_object.find("Options"); it != json_object.end())
    {
        std::vector<JSONToken> options = it->second.as_array();
        for (JSONToken& option : options)
        {
            combo->add_option(option.as_string());
        }
    }
    return combo;
});
