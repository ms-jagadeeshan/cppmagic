#include "cppmagic/Utils.h"

#include <fstream>

////////////////////////////////////////////////////////////////////////

std::string cmg::utils::join(const cmgVector<std::string>& strings, const std::string delimiter)
{
    if (strings.empty())
        return "";

    std::string out = strings.front();
    for (int i = 1; i < strings.size(); ++i)
    {
        out += delimiter + strings[i];
    }
    return out;
}

////////////////////////////////////////////////////////////////////////

cmgVector<std::string> cmg::utils::readLinesFromFile(const std::string& filename)
{
    if (filename.empty())
        return {};

    std::ifstream file(filename);
    if (!file.is_open())
        return {};

    std::vector<std::string> fileLines;
    std::string line;
    while (std::getline(file, line))
    {
        fileLines.push_back(line);
    }
    file.close();
    return fileLines;
}

////////////////////////////////////////////////////////////////////////
