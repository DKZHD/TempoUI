#include "FileReader.h"
#include <sstream>
#include <fstream>

namespace TempoUI
{
    std::string FileReader::read_File(std::string_view fileName)
    {
        std::string file_as_string = std::string(fileName);
        if (std::ifstream in_file(file_as_string); in_file)
        {
            std::stringstream str;
            str << in_file.rdbuf();
            in_file.close();
            return str.str();
        }
        return {};
    }

    bool FileReader::write_File(std::string_view fileName, const std::string& content)
    {
        std::string file_as_string = std::string(fileName);
        if (std::ofstream out_file(file_as_string); out_file)
        {
            out_file << content;
            out_file.close();
            return true;
        }
        return false;
    }
} // namespace TempoUI
