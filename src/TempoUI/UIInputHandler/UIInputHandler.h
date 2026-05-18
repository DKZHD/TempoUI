#pragma once
#ifdef GLFW_UI
#include <GLFW/glfw3.h>
#endif

namespace TempoUI // NOLINT
{
    // clang-format off
    namespace UI
    {
    enum class Action : uint8_t
    {
        Press,
        Release,
        Repeat
    };
    enum class MouseButton : uint8_t
    {
        Left,
        Right,
        Middle,
        INVALID_KEY_TYPE
    };
    enum class Key : uint8_t
    {
        A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
        Backspace, Enter, Space, Escape, Left, Right, Up, Down, INVALID_KEY_TYPE
    };
// clang-format on
#ifdef GLFW_UI
        namespace GLFW
        {
            Key translate_key(int key);
            MouseButton translate_mouse_button(int button);
            void handle_key(int key, int action);
            void handle_mouse_button(int button, int action);
            void init_input(GLFWwindow* window);
        } // namespace GLFW
#endif
    } // namespace UI
} // namespace TempoUI
