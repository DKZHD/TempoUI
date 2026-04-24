#pragma once
#include "Elements/Canvas/Canvas.h"

class LayoutLoader
{
public:
    static std::unique_ptr<Canvas> load_layout(const std::string& layout_name);

private:
};