#pragma once
#include "Elements/Canvas/Canvas.h"
#include "UIMesh/UIMesh.h"

template <typename T, typename ... Args>
T* UIRenderer::add_element(Args&&... args)
{
    return canvas->create_child<T>(args...);
}

template <typename T>
T* UIRenderer::get_element(const std::string& id)
{
    return canvas->get_element<T>(id);
}
