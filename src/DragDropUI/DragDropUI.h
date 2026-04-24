#pragma once

class ElementBase;
struct Vertex;

namespace DragDropUI
{
    struct ElementInfo
    {
        std::vector<Vertex> vertices_;
        std::vector<uint32_t> indices_;
    };

    ElementInfo get_element_info();
    void begin_drag_drop(std::unique_ptr<ElementBase> element);
    [[nodiscard]] std::unique_ptr<ElementBase> end_drag_drop();
    void handle_drag_drop();
    inline static bool s_is_moveable = false;
} // namespace DragDropUI