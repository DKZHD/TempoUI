#pragma once
#include <map>

class JSONToken;

using JSON_NULL = std::monostate;
using JSON_BOOL = bool;
using JSON_NUMBER = double;
using JSON_STRING = std::string;
using JSON_ARRAY = std::vector<JSONToken>;
using JSON_OBJECT = std::map<std::string, JSONToken>;

class JSONToken
{
public:
    JSONToken() : value(JSON_NULL{}) {}
    JSONToken(bool b) : value(b) {}
    JSONToken(double d) : value(d) {}
    JSONToken(int i) : value(static_cast<double>(i)) {}
    JSONToken(const std::string& s) : value(s) {}
    JSONToken(const char* s) : value(std::string(s)) {}
    JSONToken(const JSON_ARRAY& arr) : value(arr) {}
    JSONToken(const JSON_OBJECT& obj) : value(obj) {}

    [[nodiscard]] bool is_null() const { return std::holds_alternative<JSON_NULL>(value); }
    [[nodiscard]] bool is_bool() const { return std::holds_alternative<JSON_BOOL>(value); }
    [[nodiscard]] bool is_number() const { return std::holds_alternative<JSON_NUMBER>(value); }
    [[nodiscard]] bool is_string() const { return std::holds_alternative<JSON_STRING>(value); }
    [[nodiscard]] bool is_array() const { return std::holds_alternative<JSON_ARRAY>(value); }
    [[nodiscard]] bool is_object() const { return std::holds_alternative<JSON_OBJECT>(value); }

    [[nodiscard]] const JSON_OBJECT& as_object() const
    {
        if (!is_object())
            throw std::runtime_error("Not an Object");
        return std::get<JSON_OBJECT>(value);
    }
    [[nodiscard]] JSON_OBJECT& as_object_ref()
    {
        if (!is_object())
            throw std::runtime_error("Not an Object");
        return std::get<JSON_OBJECT>(value);
    }
    [[nodiscard]] const JSON_ARRAY& as_array() const
    {
        if (!is_array())
            throw std::runtime_error("Not an ARRAY");
        return std::get<JSON_ARRAY>(value);
    }
    [[nodiscard]] JSON_ARRAY& as_array_ref()
    {
        if (!is_array())
            throw std::runtime_error("Not an ARRAY");
        return std::get<JSON_ARRAY>(value);
    }
    [[nodiscard]] glm::vec2 as_vec2() const
    {
        if (!is_array() || as_array().size() < 2)
            throw std::runtime_error("Not an ARRAY");
        const std::vector<JSONToken>& arr = as_array();
        return { arr[0].as_number(), arr[1].as_number() };
    }
    [[nodiscard]] glm::vec3 as_vec3() const
    {
        if (!is_array() || as_array().size() < 3)
            throw std::runtime_error("Not an ARRAY");
        const std::vector<JSONToken>& arr = as_array();
        return { arr[0].as_number(), arr[1].as_number(), arr[2].as_number() };
    }
    [[nodiscard]] glm::vec4 as_vec4() const
    {
        if (!is_array() || as_array().size() < 4)
            throw std::runtime_error("Not an ARRAY");
        const std::vector<JSONToken>& arr = as_array();
        return { arr[0].as_number(), arr[1].as_number(), arr[2].as_number(), arr[3].as_number() };
    }
    [[nodiscard]] const std::string& as_string() const
    {
        if (!is_string())
            throw std::runtime_error("Not a STRING");
        return std::get<JSON_STRING>(value);
    }
    [[nodiscard]] double as_number() const
    {
        if (!is_number())
            throw std::runtime_error("Not a NUMBER");
        return std::get<JSON_NUMBER>(value);
    }
    [[nodiscard]] bool as_bool() const
    {
        if (!is_bool())
            throw std::runtime_error("Not a NUMBER");
        return std::get<JSON_BOOL>(value);
    }

private:
    std::variant<JSON_NULL, JSON_BOOL, JSON_NUMBER, JSON_STRING, JSON_ARRAY, JSON_OBJECT> value;
};

namespace JSON
{
    [[nodiscard]] std::map<std::string, JSONToken> Parse(const std::string& json_data);
}