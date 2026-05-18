#pragma once
#include <unordered_map>
#include "JsonParser/JsonParser.h"

namespace TempoUI
{
    class ThemeManager
    {
    public:
        static ThemeManager& get();
        void load_theme(const std::string& data);

        JSONToken* try_get(const std::string& s);
        bool is_loaded();
        bool exists(const std::string& s);
        JSONToken operator[](const std::string& s);

    private:
        ThemeManager() = default;
        ~ThemeManager() = default;
        std::unordered_map<std::string, JSONToken> theme;
    };
} // namespace TempoUI
