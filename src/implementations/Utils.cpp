#include <fstream>
#include "../headers/Utils.h"

std::string ReadFile(const char *filePath)
{

    std::string fileContents;

    if (std::ifstream FileStream{filePath, std::ios::in})
    {
        fileContents.assign(std::istreambuf_iterator<char>(FileStream),
             std::istreambuf_iterator<char>());
    }

    return fileContents;
}