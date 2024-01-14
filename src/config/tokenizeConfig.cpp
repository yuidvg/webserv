#include "tokenizeConfig.hpp"

TokensResult tokenize(const char *configPath)
{
    std::ifstream configFile(configPath);
    std::vector<std::string> tokens;
    std::string line, token;

    if (!configFile.is_open())
    {
        return TokensResult::Error("Failed to open file: " + std::string(configPath));
    }

    while (std::getline(configFile, line))
    {
        std::istringstream lineStream(line);
        while (lineStream >> token)
        {
            tokens.push_back(token);
        }
    }

    configFile.close();
    return TokensResult::Success(tokens);
}

TokensResult extractADirective(std::vector<std::string> &tokens)
{
    std::vector<std::string> directiveTokens;
    std::string token;

    while (!tokens.empty())
    {
        std::string currentToken = tokens.front();
        tokens.erase(tokens.begin());

        for (size_t i = 0; i < currentToken.size(); ++i)
        {
            char ch = currentToken[i];

            if (ch == ';' || ch == '{' || ch == '}')
            {
                if (ch == '{' || ch == '}')
                {
                    directiveTokens.push_back(std::string(1, ch));
                }
                else if (ch == ';')
                {
                    directiveTokens.push_back(token);
                }
                return TokensResult::Success(directiveTokens);
            }
            else
            {
                token += ch;
            }
        }

        if (!token.empty())
        {
            directiveTokens.push_back(token);
            token.clear();
        }
    }

    return TokensResult::Error("Config: ディレクティブが正常に終了しませんでした");
}
