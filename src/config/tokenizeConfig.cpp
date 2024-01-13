#include "tokenizeConfig.hpp"

/*
字句解析をした後、形式的な構文エラーと構造的な問題をチェックする。
*/
TokenResult tokenize(const char *configPath)
{
    std::ifstream configFile(configPath);
    std::string line;
    std::vector<Token> tokens;
    if (!configFile.is_open())
        return TokenResult::Err("Failed to open file: " + std::string(configPath));
    if (configFile.peek() == std::ifstream::traits_type::eof())
    {

        return TokenResult::Err("Empty file: " + std::string(configPath));
    }

    while (std::getline(configFile, line))
    {
        std::istringstream iss(line);
        Token token;
        std::string tmpStr;
        while (iss >> tmpStr)
        {
            if (tmpStr[0] == '#')
                break;
            else
                token.values.push_back(tmpStr);
        }
        if (!token.values.empty())
            tokens.push_back(token);
    }
    return TokenResult::Ok(tokens);
}
