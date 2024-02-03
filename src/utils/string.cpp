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

std::vector<std::string> tokenize(const std::string &original, const char &delim)
{
    Strings tokens;
    std::stringstream ss(original);
    std::string token;
    while (std::getline(ss, token, delim))
        if (!token.empty())
            tokens.push_back(token);
    return tokens;
}

std::string hexToUtf8Char(const std::string &hex)
{
    std::stringstream ss;
    ss << std::hex << hex;
    int dec;
    ss >> dec;
    // 10進数をcharに変換
    std::string decStr;
    decStr += static_cast<char>(dec);
    return decStr;
}

std::string getlineCustom(std::istringstream &requestTextStream)
{
    std::string line;

    std::getline(requestTextStream, line);
    if (line[line.length() - 1] == '\r')
        line.erase(line.length() - 1);
    return line;
}

Strings split(const std::string &original, const std::string &delim)
{
    Strings tokens;
    size_t start = 0;
    size_t end = original.find(delim);

    while (end != std::string::npos)
    {
        tokens.push_back(original.substr(start, end - start));
        start = end + delim.length();
        end = original.find(delim, start);
    }
    tokens.push_back(original.substr(start, end));
    return tokens;
}
} // namespace utils
