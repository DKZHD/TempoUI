#include "ElementBase.h"
#include "EventManager/EventManager.h"
#include "UIMesh/UIMesh.h"
#include <queue>
#include "JsonWriter/JsonWriter.h"

ElementBase::ElementBase(std::string id, uint32_t flags, int flex) : ID(std::move(id)), custom_values(flags), flex_(flex)
{
}

ElementBase::~ElementBase()
{
}

void ElementBase::remove_element(std::string_view id)
{
    std::queue<int> indexes_to_remove;

    for (const auto& [i, child] : std::views::enumerate(children))
    {
        if (child->ID == id)
        {
            indexes_to_remove.emplace(i);
        }
        child->remove_element(id);
    }
    for (int i = 0; i < indexes_to_remove.size(); ++i)
    {
        int index = indexes_to_remove.front();
        indexes_to_remove.pop();

        children.erase(children.begin() + (index - i));
    }
}

void ElementBase::update_absolute_position()
{
    absolute_position = get_position();
}

void ElementBase::set_local_position(glm::vec2 position)
{
    config.localPosition = position;
}
void ElementBase::set_size(glm::vec2 size)
{
    config.size = size;
}

ElementBase* ElementBase::get_parent() const
{
    return parent;
}

glm::vec2 ElementBase::get_position() const
{
    if (parent != nullptr)
    {
        return config.localPosition + parent->get_position();
    }
    return config.localPosition;
}

const glm::vec2& ElementBase::get_local_position() const
{
    return config.localPosition;
}

glm::vec2 ElementBase::get_size() const
{
    return config.size;
}

std::vector<Vertex>& ElementBase::get_vertices()
{
    return vertices;
}

const std::vector<unsigned int>& ElementBase::get_indices() const
{
    return indices;
}

std::vector<std::unique_ptr<ElementBase>>& ElementBase::get_children()
{
    return children;
}

void ElementBase::render()
{
    if (!(config.state & VISIBLE))
    {
        return;
    }

    for (const auto& child : children)
    {
        child->render();
    }
}

bool ElementBase::isHovered()
{
    glm::vec2 mouse_position = EventManager::get().get_mouse_position();
    glm::vec2 position = absolute_position;
    return (config.state & HOVERABLE) && (config.state & VISIBLE) && mouse_position.x >= position.x && mouse_position.y >= position.y &&
           mouse_position.x <= position.x + config.size.x && mouse_position.y <= position.y + config.size.y;
}

void ElementBase::theme_updated()
{
    self_theme_updated();
    for (auto& child : children)
    {
        child->theme_updated();
    }
    mark_dirty();
}

void ElementBase::scale_updated(glm::vec2 new_scale)
{
    if (original_size.x == FLT_MAX)
    {
        original_size = config.size;
    }
    config.size = original_size * new_scale;
    config.radius = (config.radius / global_scale_.y) * new_scale.y;
    global_scale_ = new_scale;
    self_scale_updated(new_scale);
    for (auto& child : children)
    {
        child->scale_updated(new_scale);
    }
    mark_dirty();
}

void ElementBase::serialize(JsonWriter& writer)
{
    writer.start_object();
    writer.write_string("Id", ID);
    writer.write_vec2("Pos", config.localPosition);
    writer.write_vec2("Size", config.size);
    if (config.radius != glm::vec4(0.f))
    {
        writer.write_vec4("Radius", config.radius);
    }
    if (flex_ > 0)
    {
        writer.write_float("Flex", (float)flex_);
    }

    self_serialize(writer);

    if (!children.empty())
    {
        writer.start_array("Children");
        for (const auto& child : children)
        {
            child->serialize(writer);
        }
        writer.end_array();
    }
    writer.end_object();
}

std::unique_ptr<ElementBase> ElementBase::release_hovered_element()
{
    if (!isHovered())
    {
        return nullptr;
    }
    for (int i = (int)children.size() - 1; i >= 0; i--)
    {
        if (children[i]->isHovered())
        {
            std::unique_ptr<ElementBase> descendant = children[i]->release_hovered_element();

            if (descendant)
            {
                return descendant;
            }

            std::unique_ptr<ElementBase> plucked_target = std::move(children[i]);

            children.erase(children.begin() + i);

            glm::vec2 absolute_screen_pos = plucked_target->get_position();
            plucked_target->set_local_position(absolute_screen_pos);
            plucked_target->parent = nullptr;

            this->mark_dirty();
            return plucked_target;
        }
    }

    return nullptr;
}

ElementBase* ElementBase::get_hovered_element()
{
    if (!isHovered())
    {
        return nullptr;
    }

    for (int i = (int)children.size() - 1; i >= 0; --i)
    {
        ElementBase* found = children[i]->get_hovered_element();
        if (!children[i]->check_state(IS_OVERLAY) && found)
        {
            return found;
        }
    }
    return this;
}

ElementBase* ElementBase::get_hovered_overlay()
{
    if (!check_state(VISIBLE) || !check_state(HOVERABLE))
        return nullptr;
    if (check_state(IS_OVERLAY))
    {
        return get_hovered_element();
    }

    for (int i = (int)children.size() - 1; i >= 0; --i)
    {
        if (ElementBase* found = children[i]->get_hovered_overlay())
            return found;
    }
    return nullptr;
}

void ElementBase::on_hover()
{
    if (config.state & HOVERABLE)
        config.state |= IS_HOVERED;
    if (on_hover_func)
        (*on_hover_func)();
}

void ElementBase::on_unhover()
{
    if (config.state & HOVERABLE)
        config.state &= ~IS_HOVERED;
    if (on_unhover_func)
        (*on_unhover_func)();
}

void ElementBase::set_on_hover(std::function<void()> func)
{
    on_hover_func = std::make_unique<std::function<void()>>(std::move(func));
}

void ElementBase::set_on_unhover(std::function<void()> func)
{
    on_unhover_func = std::make_unique<std::function<void()>>(std::move(func));
}

void ElementBase::on_key(UI::Key key, UI::Action action)
{
    self_on_key(key, action);
    for (const auto& child : children)
    {
        child->on_key(key, action);
    }
}

void ElementBase::on_mouse_button(UI::MouseButton button, UI::Action action)
{
    self_on_mouse_button(button, action);
    for (const auto& child : children)
    {
        child->on_mouse_button(button, action);
    }
}

void ElementBase::on_char(const char character)
{
    self_on_char(character);
    for (const auto& child : children)
    {
        child->on_char(character);
    }
}

void ElementBase::on_scroll(float yOffset)
{
    self_on_scroll(yOffset);
    for (const auto& child : children)
    {
        child->on_scroll(yOffset);
    }
}

void ElementBase::update(const float dt)
{
    self_update(dt);
    for (const auto& child : children)
    {
        child->update(dt);
    }
}

bool ElementBase::get_is_dirty() const
{
    return is_dirty;
}

bool ElementBase::check_state(ElementState state) const
{
    return (config.state & state);
}

void ElementBase::toggle_state(ElementState state)
{
    config.state ^= state;
}

void ElementBase::set_state(ElementState state, bool value)
{
    if (value)
    {
        config.state |= state;
    }
    else
    {
        config.state &= ~state;
    }
}

void ElementBase::mark_dirty()
{
    is_dirty = true;
    absolute_position = get_position();
    update_buffers();
    if (parent != nullptr)
    {
        parent->mark_dirty();
    }
}

void ElementBase::clear_dirty()
{
    is_dirty = false;
    for (const auto& child : children)
    {
        child->clear_dirty();
    }
}
