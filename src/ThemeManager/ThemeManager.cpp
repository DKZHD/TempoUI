#include "ThemeManager.h"

void ThemeManager::load_theme(const std::string& data)
{
    std::map<std::string, JSONToken> mapped_data = JSON::Parse(data);
    theme = { mapped_data.begin(), mapped_data.end() };
}

JSONToken* ThemeManager::try_get(const std::string& s)
{
    auto it = theme.find(s);
    if (it == theme.end())
    {
        return nullptr;
    }
    return &it->second;
}

bool ThemeManager::is_loaded()
{
    return !theme.empty();
}

bool ThemeManager::exists(const std::string& s)
{
    return theme.contains(s);
}

JSONToken ThemeManager::operator[](const std::string& s)
{
    auto it = theme.find(s);
    if (it == theme.end())
    {
        std::cout << "Couldn't find value for key: " << s << '\n';
        return {};
    }
    return it->second;
}

ThemeManager& ThemeManager::get()
{
    static ThemeManager instance;
    return instance;
}
