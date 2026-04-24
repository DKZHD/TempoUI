#include "DragDropUI.h"
#include "Elements/ElementBase/ElementBase.h"
#include "EventManager/EventManager.h"
#include "UIMesh/UIMesh.h"

namespace
{
    std::unique_ptr<ElementBase> current_held_element;
    glm::vec2 drag_offset = glm::vec2(0.f);

    void update_tree_buffers(ElementBase* element)
    {
        if (!element)
            return;

        element->update_absolute_position();
        element->update_buffers();

        for (auto& child : element->get_children())
        {
            update_tree_buffers(child.get());
        }
    }

    void append_element_geometry(ElementBase* element, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
    {
        if (!element)
            return;

        for (unsigned int idx : element->get_indices())
        {
            indices.push_back(idx + vertices.size());
        }
        vertices.insert(vertices.end(), element->get_vertices().begin(), element->get_vertices().end());

        for (const auto& child : element->get_children())
        {
            append_element_geometry(child.get(), vertices, indices);
        }
    }
} // namespace

DragDropUI::ElementInfo DragDropUI::get_element_info() // NOLINT
{
    if (!current_held_element)
        return {};

    update_tree_buffers(current_held_element.get());
    DragDropUI::ElementInfo info;
    append_element_geometry(current_held_element.get(), info.vertices_, info.indices_);
    return info;
}

void DragDropUI::handle_drag_drop()
{
    if (!current_held_element)
        return;

    glm::dvec2 mouse_pos = EventManager::get().get_mouse_position();
    glm::vec2 new_pos = glm::vec2(mouse_pos) - drag_offset;

    current_held_element->set_local_position(new_pos);
    current_held_element->mark_dirty();
}

void DragDropUI::begin_drag_drop(std::unique_ptr<ElementBase> element)
{
    current_held_element = std::move(element);
    glm::dvec2 mouse_pos = EventManager::get().get_mouse_position();
    glm::vec2 element_pos = current_held_element->get_position();

    drag_offset = glm::vec2(mouse_pos.x, mouse_pos.y) - element_pos;
}
std::unique_ptr<ElementBase> DragDropUI::end_drag_drop()
{
    std::unique_ptr<ElementBase> temp = std::move(current_held_element);
    current_held_element = nullptr;
    return std::move(temp);
}
