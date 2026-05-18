#pragma once

namespace TempoUI
{
    class JsonWriter
    {
    public:
        JsonWriter();

        void start_object(const std::string& id = "");
        void end_object();

        void start_array(const std::string& key = "");
        void end_array();

        void write_string(const std::string& key, const std::string& value);
        void write_float(const std::string& key, float value);
        void write_bool(const std::string& key, bool value);

        void write_vec2(const std::string& key, const glm::vec2& vec);
        void write_vec4(const std::string& key, const glm::vec4& vec);

        std::string get_string() const;

    private:
        std::stringstream ss_;
        int indent_level_ = 0;

        std::vector<bool> needs_comma_;
        void write_indent();
        void prepare_value();
    };
} // namespace TempoUI
