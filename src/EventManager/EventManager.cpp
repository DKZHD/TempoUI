#include "EventManager.h"

EventManager::EventManager()
{
}

EventManager& EventManager::get()
{
    static EventManager instance;
    return instance;
}

EventManager::~EventManager()
{
}

void EventManager::bind_key_event(std::function<void(UI::Key, UI::Action)> func)
{
    key_events_.emplace_back(std::move(func));
}

void EventManager::bind_mouse_button_event(std::function<void(UI::MouseButton, UI::Action)> func)
{
    mouse_events_.emplace_back(std::move(func));
}

void EventManager::bind_scroll_event(std::function<void(double)> func)
{
    scroll_events_.emplace_back(std::move(func));
}

void EventManager::bind_char_event(std::function<void(char)> func)
{
    char_events_.emplace_back(std::move(func));
}

void EventManager::inject_key(UI::Key key, UI::Action action)
{
    for (auto& event : key_events_)
    {
        event(key, action);
    }
}

void EventManager::inject_mouse_button(UI::MouseButton button, UI::Action action)
{
    for (auto& event : mouse_events_)
    {
        event(button, action);
    }
}

void EventManager::inject_char(char character)
{
    for (auto& func : char_events_)
    {
        func(character);
    }
}

void EventManager::inject_scroll(double y_offset)
{
    for (auto& func : scroll_events_)
    {
        func(y_offset);
    }
}

void EventManager::inject_mouse_position(double x, double y)
{
    mouse_pos = glm::dvec2(x, y);
}

const glm::dvec2& EventManager::get_mouse_position() const
{
    return mouse_pos;
}