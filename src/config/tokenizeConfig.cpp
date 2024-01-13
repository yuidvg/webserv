#include "tokenizeConfig.hpp"

/*
字句解析をした後、形式的な構文エラーと構造的な問題をチェックする。
*/
TokenizeResult tokenize(const char *configPath)
{
    std::ifstream configFile(configPath);
    std::string line;
    std::vector<Tokenize> tokens;
    if (!configFile.is_open())
        return TokenizeResult::Err("Failed to open file: " + std::string(configPath));
    if (configFile.peek() == std::ifstream::traits_type::eof())
    {

        return TokenizeResult::Err("Empty file: " + std::string(configPath));
    }

    while (std::getline(configFile, line))
    {
        std::istringstream iss(line);
        Tokenize token;
        std::string tmpStr;
        while (iss >> tmpStr)
        {
            if (tmpStr[0] == '#')
                break;
            if (token.key.empty())
                token.key = tmpStr;
            else
                token.values.push_back(tmpStr);
        }
        if (!token.key.empty())
        {
            // if (!token.values.empty())
            // {
            //     StringResult result = removeSemicolonFromToken(token);
            //     if (!result.ok())
            //         return TokenizeResult::Err(result.unwrapErr());
            // }
            tokens.push_back(token);
        }
    }
    return TokenizeResult::Ok(tokens);
}
