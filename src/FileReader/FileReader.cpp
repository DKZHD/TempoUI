#include "FileReader.h"
#include <sstream>
#include <fstream>

std::string FileReader::read_File(std::string_view fileName)
{
    std::string fileAsString = std::string(fileName);
    if (std::ifstream inFile(fileAsString); inFile)
    {
        std::stringstream str;
        str << inFile.rdbuf();
        inFile.close();
        return str.str();
    }
    return {};
}

bool FileReader::write_File(std::string_view fileName, const std::string& content)
{
    std::string fileAsString = std::string(fileName);
    if (std::ofstream outFile(fileAsString); outFile)
    {
        outFile << content;
        outFile.close();
        return true;
    }
    return false;
}

