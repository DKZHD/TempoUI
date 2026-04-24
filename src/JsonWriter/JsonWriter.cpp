#include "JSONWriter.h"

JsonWriter::JsonWriter()
{
    needs_comma_.push_back(false);
}

void JsonWriter::write_indent()
{
    ss_ << std::string(static_cast<long long>(indent_level_) * 4, ' ');
}
void JsonWriter::prepare_value()
{
    if (needs_comma_.back())
    {
        ss_ << ",\n";
    }
    else
    {
        ss_ << '\n';
        needs_comma_.back() = true;
    }
    write_indent();
}

void JsonWriter::start_object(const std::string& id)
{
    prepare_value();
    if (!id.empty())
        ss_ << "\"" << id << "\": ";
    ss_ << '{';

    indent_level_++;
    needs_comma_.push_back(false);
}
void JsonWriter::end_object()
{
    indent_level_--;
    needs_comma_.pop_back();
    ss_ << '\n';
    write_indent();
    ss_ << '}';
}

void JsonWriter::start_array(const std::string& key)
{
    prepare_value();
    if (!key.empty())
        ss_ << "\"" << key << "\": ";
    ss_ << "[";

    indent_level_++;
    needs_comma_.push_back(false);
}

void JsonWriter::end_array()
{
    indent_level_--;
    needs_comma_.pop_back();
    ss_ << '\n';
    write_indent();
    ss_ << "]";
}

void JsonWriter::write_string(const std::string& key, const std::string& value)
{
    prepare_value();
    ss_ << "\"" << key << "\": \"" << value << "\"";
}

void JsonWriter::write_float(const std::string& key, float value)
{
    prepare_value();
    ss_ << "\"" << key << "\": " << value;
}

void JsonWriter::write_vec2(const std::string& key, const glm::vec2& vec)
{
    prepare_value();
    ss_ << '"' << key << '"' << ": " << '[' << vec.x << ", " << vec.y << ']';
}

void JsonWriter::write_vec4(const std::string& key, const glm::vec4& vec)
{
    prepare_value();
    ss_ << '"' << key << '"' << ": " << '[' << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ']';
}

std::string JsonWriter::get_string() const
{
    return ss_.str();
}