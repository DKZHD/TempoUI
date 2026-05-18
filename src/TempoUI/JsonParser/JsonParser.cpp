#include "JsonParser.h"
#include <algorithm>
#include <cctype>

namespace TempoUI
{
    namespace
    {
        JSONToken parse_value(const std::string_view& s, size_t& pos);

        std::pair<size_t, size_t> calculate_limits(char first, char last, const std::string_view& s, size_t offset)
        {
            size_t begin = s.find_first_of(first, offset);
            size_t end = s.find_first_of(last, begin);
            return { begin, end };
        }

        void skip_whitespace(const std::string_view& s, size_t& pos)
        {
            while (pos < s.length() && std::isspace(s[pos]))
                pos++;
        }

        bool is_digit_s(const std::string_view& s)
        {
            return std::ranges::all_of(s, [](char c)
            {
                return (bool)isdigit(c) || c == '-' || c == '.';
            });
        }

        double parse_number(const std::string_view& s, size_t& pos)
        {
            size_t end = s.find_first_of(",] }\t\n\r", pos);
            std::string sub(s.substr(pos, end - pos));
            pos = end;
            if (is_digit_s(sub))
            {
                return std::stod(sub);
            }
            return -DBL_MAX;
        }

        std::string parse_string(const std::string_view& s, size_t& pos)
        {
            pos++;
            size_t end = s.find_first_of('"', pos);
            std::string str(s.substr(pos, end - pos));
            pos = end + 1;
            return str;
        }

        JSONToken handle_array(const std::string_view& s, size_t& current_pos)
        {
            std::vector<JSONToken> tokens;
            current_pos++;
            while (current_pos < s.length())
            {
                current_pos = s.find_first_not_of(" \t\n\r", current_pos);
                if (current_pos == std::string_view::npos)
                    break;

                if (s[current_pos] == ']')
                {
                    current_pos++;
                    break;
                }

                tokens.emplace_back(parse_value(s, current_pos));

                current_pos = s.find_first_not_of(" \t\n\r", current_pos);
                if (current_pos == std::string_view::npos)
                    break;
                if (s[current_pos] == ',')
                {
                    current_pos++;
                }
            }
            return tokens;
        }

        JSONToken handle_object(const std::string_view& s, size_t& current_pos)
        {
            std::map<std::string, JSONToken> tokens;
            current_pos++;
            while (current_pos < s.length())
            {
                current_pos = s.find_first_not_of(" \t\n\r", current_pos);
                if (current_pos == std::string_view::npos)
                    break;

                if (s[current_pos] == '}')
                {
                    current_pos++;
                    break;
                }
                size_t name_length = s.find_first_of('"', current_pos + 1);
                std::string name(s.substr(current_pos + 1, name_length - (current_pos + 1)));
                current_pos = name_length + 1;

                current_pos = s.find_first_not_of(": \n\r\t", current_pos);
                if (current_pos == std::string_view::npos)
                    break;
                tokens.emplace(name, parse_value(s, current_pos));

                current_pos = s.find_first_not_of(" \t\n\r", current_pos);
                if (current_pos == std::string_view::npos)
                    break;
                if (s[current_pos] == ',')
                {
                    current_pos++;
                }
            }
            return tokens;
        }

        JSONToken parse_value(const std::string_view& s, size_t& pos)
        {
            if (s.empty())
            {
                return {};
            }
            skip_whitespace(s, pos);
            char c = s[pos];
            if (c == '{')
            {
                return handle_object(s, pos);
            }
            if (c == '[')
            {
                return handle_array(s, pos);
            }
            if (isdigit(c) || c == '-')
            {
                return parse_number(s, pos);
            }
            if (c == '"')
            {
                return parse_string(s, pos);
            }
            if (c == 't')
            {
                pos += 4;
                return true;
            }
            if (c == 'f')
            {
                pos += 5;
                return false;
            }
            if (c == 'n')
            {
                pos += 4;
                return {};
            }
            return {};
        }

    } // namespace

    namespace JSON
    {
        std::map<std::string, JSONToken> Parse(const std::string& json_data)
        {
            size_t current_pos = 0;

            JSONToken root = parse_value(json_data, current_pos);
            if (!root.is_object())
                return {};

            JSON_OBJECT& obj = root.as_object_ref();
            if (auto it = obj.find("Globals"); it != obj.end())
            {
                JSON_OBJECT globals = it->second.as_object();
                for (auto& [obj_key, obj_value] : obj)
                {
                    if (obj_key == "Globals" || !obj_value.is_object())
                        continue;

                    JSON_OBJECT& current_obj = obj_value.as_object_ref();
                    for (auto& [param_name, param_value] : current_obj)
                    {
                        if (param_value.is_string())
                        {
                            std::string_view str = param_value.as_string();
                            if (!str.empty() && str[0] == '$')
                            {
                                std::string final_string(str.substr(1));
                                if (globals.contains(final_string))
                                {
                                    param_value = globals.at(final_string);
                                }
                                else
                                {
                                    std::cout << "Global doesn't exist: " << final_string << '\n';
                                }
                                continue;
                            }
                        }
                    }
                }
            }
            return obj;
        }
    } // namespace JSON
} // namespace TempoUI
