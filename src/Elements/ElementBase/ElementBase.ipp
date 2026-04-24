#pragma once
#include <utility>
#include "ElementBase.h"

template <typename Derived>
Derived& ElementConfig<Derived>::id(std::string id)
{
    id_ = std::move(id);
    return static_cast<Derived&>(*this);
}

template <typename Derived>
Derived& ElementConfig<Derived>::position(glm::vec2 pos)
{
    pos_ = pos;
    return static_cast<Derived&>(*this);
}

template <typename Derived>
Derived& ElementConfig<Derived>::size(glm::vec2 size)
{
    custom_values |= CustomFlags::Size;
    this->size_ = size;
    return static_cast<Derived&>(*this);
}

template <typename Derived>
Derived& ElementConfig<Derived>::radius(glm::vec4 radius)
{
    custom_values |= CustomFlags::Radius;
    this->radius_ = radius;
    return static_cast<Derived&>(*this);
}

template <typename Derived>
Derived& ElementConfig<Derived>::flex(int flex)
{
    custom_values |= CustomFlags::Flex;
    this->flex_ = flex;
    return static_cast<Derived&>(*this);
}

template <typename T>
T* ElementBase::add_child(std::unique_ptr<T> element)
{
    element->parent = this;
    T* temp = element.get();
    children.push_back(std::move(element));
    children.back()->mark_dirty();
    return temp;
}

template <typename T, typename... Args>
T* ElementBase::create_child(Args&&... args)
{
    return add_child(std::make_unique<T>(std::forward<Args>(args)...));
}

template <typename T>
T* ElementBase::get_element(const std::string_view id)
{
    if (ID == id)
    {
        return dynamic_cast<T*>(this);
    }

    for (const auto& child : children)
    {
        T* return_value = child->get_element<T>(id);
        if (return_value != nullptr)
        {
            return return_value;
        }
    }
    return nullptr;
}
