#pragma once

class FileReader
{
public:
    static std::string read_File(std::string_view fileName);
    static bool write_File(std::string_view fileName, const std::string& content);
};
