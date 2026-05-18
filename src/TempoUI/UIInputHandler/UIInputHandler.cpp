#include "UIInputHandler.h"
#include "EventManager/EventManager.h"
namespace TempoUI // NOLINT
{
#ifdef GLFW_UI
    namespace UI::GLFW
    {
        Key translate_key(int key)
        {
            // clang-format off
        switch (key)
        {
        case GLFW_KEY_A: return Key::A;
        case GLFW_KEY_B: return Key::B;
        case GLFW_KEY_C: return Key::C;
        case GLFW_KEY_D: return Key::D;
        case GLFW_KEY_E: return Key::E;
        case GLFW_KEY_F: return Key::F;
        case GLFW_KEY_G: return Key::G;
        case GLFW_KEY_H: return Key::H;
        case GLFW_KEY_I: return Key::I;
        case GLFW_KEY_J: return Key::J;
        case GLFW_KEY_K: return Key::K;
        case GLFW_KEY_L: return Key::L;
        case GLFW_KEY_M: return Key::M;
        case GLFW_KEY_N: return Key::N;
        case GLFW_KEY_O: return Key::O;
        case GLFW_KEY_P: return Key::P;
        case GLFW_KEY_Q: return Key::Q;
        case GLFW_KEY_R: return Key::R;
        case GLFW_KEY_S: return Key::S;
        case GLFW_KEY_T: return Key::T;
        case GLFW_KEY_U: return Key::U;
        case GLFW_KEY_V: return Key::V;
        case GLFW_KEY_W: return Key::W;
        case GLFW_KEY_X: return Key::X;
        case GLFW_KEY_Y: return Key::Y;
        case GLFW_KEY_Z: return Key::Z;
    
        case GLFW_KEY_ESCAPE: return Key::Escape;
        case GLFW_KEY_BACKSPACE: return Key::Backspace;
        case GLFW_KEY_ENTER: return Key::Enter;
        case GLFW_KEY_UP: return Key::Up;
        case GLFW_KEY_DOWN: return Key::Down;
        case GLFW_KEY_RIGHT: return Key::Right;
        case GLFW_KEY_LEFT: return Key::Left;
        default:
            break;
        }
            // clang-format on
            return Key::INVALID_KEY_TYPE;
        }

        MouseButton translate_mouse_button(int button)
        {
            switch (button)
            {
            case GLFW_MOUSE_BUTTON_LEFT:
                return MouseButton::Left;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                return MouseButton::Middle;
            case GLFW_MOUSE_BUTTON_RIGHT:
                return MouseButton::Right;
            default:
                break;
            }
            return MouseButton::INVALID_KEY_TYPE;
        }

        void handle_key(int key, int action)
        {
            auto ui_key = translate_key(key);
            UI::Action ui_action = UI::Action::Press;
            switch (action)
            {
            case GLFW_PRESS:
                ui_action = UI::Action::Press;
                break;
            case GLFW_RELEASE:
                ui_action = UI::Action::Release;
                break;
            case GLFW_REPEAT:
                ui_action = UI::Action::Repeat;
                break;
            default:
                break;
            }
            EventManager::get().inject_key(ui_key, ui_action);
        }

        void handle_mouse_button(int button, int action)
        {
            auto ui_button = translate_mouse_button(button);
            UI::Action ui_action = UI::Action::Press;
            switch (action)
            {
            case GLFW_PRESS:
                ui_action = UI::Action::Press;
                break;
            case GLFW_RELEASE:
                ui_action = UI::Action::Release;
                break;
            case GLFW_REPEAT:
                ui_action = UI::Action::Repeat;
                break;
            default:
                break;
            }
            EventManager::get().inject_mouse_button(ui_button, ui_action);
        }
        void init_input(GLFWwindow* window)
        {
            glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                UI::GLFW::handle_key(key, action);
            });
            glfwSetCharCallback(window, [](GLFWwindow* window, unsigned int character)
            {
                EventManager::get().inject_char((char)character);
            });
            glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
            {
                UI::GLFW::handle_mouse_button(button, action);
            });
            glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                EventManager::get().inject_scroll(yOffset);
            });
            glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
            {
                EventManager::get().inject_mouse_position(x, y);
            });
        }
    } // namespace UI::GLFW
#endif
} // namespace TempoUI
