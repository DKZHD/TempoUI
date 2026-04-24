#pragma once

#include "UIInputHandler/UIInputHandler.h"

class EventManager
{
public:
    static EventManager& get();
    ~EventManager();

    void bind_key_event(std::function<void(UI::Key, UI::Action)> func);
    void bind_mouse_button_event(std::function<void(UI::MouseButton, UI::Action)> func);
    void bind_scroll_event(std::function<void(double)> func);
    void bind_char_event(std::function<void(char)> func);

    void inject_key(UI::Key key, UI::Action action);
    void inject_mouse_button(UI::MouseButton button, UI::Action action);
    void inject_char(char character);
    void inject_scroll(double y_offset);
    void inject_mouse_position(double x, double y);

    [[nodiscard]] const glm::dvec2& get_mouse_position() const;

private:
    EventManager();

    std::vector<std::function<void(UI::Key, UI::Action)>> key_events_;
    std::vector<std::function<void(UI::MouseButton, UI::Action)>> mouse_events_;
    std::vector<std::function<void(double)>> scroll_events_;
    std::vector<std::function<void(char)>> char_events_;

    glm::dvec2 mouse_pos = glm::dvec2(0.0);
};