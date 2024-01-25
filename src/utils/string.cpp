#include ".hpp"

namespace utils
{

std::string &trim(std::string &str)
{
    while (std::isspace(str[0]))
        str.erase(0, 1);
    while (std::isspace(str[str.length() - 1]))
        str.erase(str.length() - 1, 1);
    return (str);
}

std::string lowerCase(std::string str)
{
    std::transform(&str[0], &str[0] + str.size(), &str[0], ::tolower);
    return (str);
}

bool isNumber(const std::string str)
{
    for (int i = 0; str[i]; i++)
    {
        if (!std::isdigit(str[i]))
            return (false);
    }
    return (true);
}

std::vector<const std::string> tokenize(const std::string &original, const char &delim)
{
    std::vector<const std::string> tokens;
    std::stringstream ss(original);
    std::string token;
    while (std::getline(ss, token, delim))
        if (!token.empty())
            tokens.push_back(token);
    return tokens;
}

} // namespace utils
