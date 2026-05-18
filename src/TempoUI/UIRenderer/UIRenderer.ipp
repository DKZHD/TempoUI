#pragma once
#include "UIRenderer.h"
// IWYU pragma: begin_keep
#include "Elements/Canvas/Canvas.h"
#include "UIMesh/UIMesh.h"
#include <utility>
// IWYU pragma: end_keep

template <typename T, typename... Args>
T* TempoUI::UIRenderer::add_element(Args&&... args)
{
    return canvas->create_child<T>(std::forward<Args>(args)...);
}

template <typename T>
T* TempoUI::UIRenderer::get_element(const std::string& id)
{
    return canvas->get_element<T>(id);
}
