#pragma once
#include "Elements/ElementBase/ElementBase.h"
#include "JsonParser/JsonParser.h"

struct BaseProperties
{
    std::optional<std::string> Id;
    std::optional<glm::vec2> Pos;
    std::optional<glm::vec2> Size;
    std::optional<glm::vec4> Radius;
    std::optional<int> Flex;
};

using reg_func = std::function<std::unique_ptr<ElementBase>(const JSON_OBJECT&, BaseProperties)>;

class LayoutRegistry
{

public:
    static std::unordered_map<std::string, reg_func>& get_registry()
    {
        static std::unordered_map<std::string, reg_func> registry;
        return registry;
    }
    struct AutoRegister
    {
        AutoRegister(const std::string& type, const reg_func& func)
        {
            get_registry()[type] = func;
        }
    };
};