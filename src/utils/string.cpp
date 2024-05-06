#include "../all.hpp"

namespace utils
{
std::string trim(const std::string &str)
{
    std::string::size_type startPos = 0;
    std::string::size_type endPos = str.length();
    // Remove leading spaces
    while (startPos < endPos && std::isspace(str[startPos]))
        ++startPos;
    // Remove trailing spaces
    while (endPos > startPos && std::isspace(str[endPos - 1]))
        --endPos;
    return str.substr(startPos, endPos - startPos);
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

std::vector<std::string> split(const std::string &original, const std::string &delim)
{
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string::size_type end = 0;
    while ((end = original.find(delim, start)) != std::string::npos)
    {
        const std::string token = original.substr(start, end - start);
        if (!token.empty())
            tokens.push_back(token);
        start = end + delim.length();
    }
    const std::string lastToken = original.substr(start);
    if (!lastToken.empty())
        tokens.push_back(lastToken);
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

std::string removeCharacter(const std::string str, const char charToRemove)
{
    size_t position = str.find(charToRemove);
    std::string result = str;
    while (position != std::string::npos)
    {
        result.erase(position, 1);
        position = result.find(charToRemove, position);
    }
    return result;
}

StringToIntResult stringToInt(const std::string &str, int minVal, int maxVal)
{
    if (!utils::isNumber(str))
    {
        return StringToIntResult::Error("値が数値ではありません");
    }

    std::istringstream iss(str);
    int num;
    if (!(iss >> num) || !iss.eof())
    {
        return StringToIntResult::Error("数値の変換に失敗しました");
    }

    if (num < minVal || num > maxVal)
    {
        return StringToIntResult::Error("数値が許容範囲外です");
    }

    return StringToIntResult::Success(num);
}

ReadFileResult readFile(const int fd)
{
    char buffer[MAX_LEN];
    ssize_t readSize;
    std::string result = "";

    while ((readSize = read(fd, buffer, MAX_LEN)) > 0)
    {
        result.append(buffer, readSize);
    }
    if (readSize == -1)
    {
        return ReadFileResult::Error("ファイルの読み込みに失敗しました");
    }
    return ReadFileResult::Success(result);
}

std::string itoa(const int &num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}

bool isHex(const std::string &str)
{
    for (int i = 0; str[i]; i++)
    {
        if (!std::isxdigit(str[i]))
            return (false);
    }
    return (true);
}

} // namespace utils
