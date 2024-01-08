#include "utils.hpp"

std::string &trim(std::string &str)
{
    while (std::isspace(str[0]))
        str.erase(0, 1);
    while (std::isspace(str[str.length() - 1]))
        str.erase(str.length() - 1, 1);
    return (str);
}

std::string toLower(std::string str)
{
    std::transform(&str[0], &str[0] + str.size(), &str[0], ::tolower);
    return (str);
}

bool isNumber(const std::string &str)
{
    for (int i = 0; str[i]; i++)
    {
        if (!std::isdigit(str[i]))
            return (false);
    }
    return (true);
}
