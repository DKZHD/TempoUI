#include "LayoutLoader.h"
#include "JsonParser/JsonParser.h"
#include "LayoutRegistry/LayoutRegistry.h"
#include <fstream>
#include <sstream>

namespace TempoUI
{
    namespace
    {
        BaseProperties extract_base_props(const JSON_OBJECT& json_object)
        {
            BaseProperties props;

            if (auto it = json_object.find("Id"); it != json_object.end())
                props.Id = it->second.as_string();
            if (auto it = json_object.find("Pos"); it != json_object.end())
                props.Pos = it->second.as_vec2();
            if (auto it = json_object.find("Size"); it != json_object.end())
                props.Size = it->second.as_vec2();
            if (auto it = json_object.find("Radius"); it != json_object.end())
                props.Radius = it->second.as_vec4();
            if (auto it = json_object.find("Flex"); it != json_object.end())
                props.Flex = it->second.as_number();

            return props;
        }

        std::unique_ptr<ElementBase> load_element(const JSON_OBJECT& json_object)
        {
            std::unique_ptr<ElementBase> element;
            auto type_it = json_object.find("Type");
            if (type_it == json_object.end())
                return nullptr;

            BaseProperties props = extract_base_props(json_object);
            std::string type_string = type_it->second.as_string();
            std::unordered_map<std::string, reg_func>& registry = LayoutRegistry::get_registry();

            if (auto it = registry.find(type_string); it != registry.end())
            {
                element = it->second(json_object, props);

                if (auto children_it = json_object.find("Children"); children_it != json_object.end())
                {
                    if (children_it->second.is_array())
                    {
                        JSON_ARRAY children_array = children_it->second.as_array();
                        for (const JSONToken& json_token : children_array)
                        {
                            if (!json_token.is_object())
                                continue;
                            JSON_OBJECT child_object = json_token.as_object();
                            element->add_child(std::move(load_element(child_object)));
                        }
                    }
                    else
                    {
                        JSON_OBJECT children_map = children_it->second.as_object();
                        for (const auto& [child_id, token] : children_map)
                        {
                            JSON_OBJECT obj = token.as_object();

                            element->add_child(std::move(load_element(token.as_object())));
                        }
                    }
                }
                return element;
            }

            return nullptr;
        }
    } // namespace

    std::unique_ptr<Canvas> LayoutLoader::load_layout(const std::string& layout_name)
    {
        std::unique_ptr<Canvas> canvas = std::make_unique<Canvas>("Layout_Canvas", glm::vec2(0.f), glm::vec2(1920, 1080));

        std::string file_contents;
        if (std::filesystem::exists(layout_name))
        {
            std::stringstream ss;
            ss << std::ifstream(layout_name).rdbuf();
            file_contents = ss.str();
        }
        else
        {
            std::ofstream outfile(layout_name);
            std::cout << "Empty Layout file created\n";
            outfile.close();
            return canvas;
        }
        std::map<std::string, JSONToken> parsed_values = JSON::Parse(file_contents);
        for (auto& [name, value] : parsed_values)
        {
            canvas->add_child(std::move(load_element(value.as_object())));
        }
        return canvas;
    }
} // namespace TempoUI
