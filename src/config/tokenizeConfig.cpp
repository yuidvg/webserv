#include "tokenizeConfig.hpp"

StringResult checkBracketsBalance(const std::vector<Tokenize> &tokens)
{
    int brackets = 0;
    for (std::vector<Tokenize>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        if (it->key == OPEN_BRACKET)
            brackets++;
        else if (it->key == CLOSE_BRACKET)
            brackets--;
        for (std::vector<std::string>::const_iterator valueIt = it->values.begin(); valueIt != it->values.end();
             ++valueIt)
        {
            if (*valueIt == OPEN_BRACKET)
                brackets++;
            else if (*valueIt == CLOSE_BRACKET)
                brackets--;
        }
        // サーバーコンテキスト内での2回ネストされたコンテキストは許可されません
        if (brackets > 2)
            return StringResult::Err("Too many nested contexts in config file");
    }
    if (brackets != 0)
        return StringResult::Err("Invalid number of brackets " + utils::to_string(brackets) + " in config file");
    return StringResult::Ok("ok");
}

StringResult removeSemicolonFromToken(Tokenize &token)
{
    if (token.key == SERVER_NAME || token.key == LISTEN || token.key == ALLOW_METHOD || token.key == CGI_EXECUTOR ||
        token.key == UPLOAD_PATH || token.key == RETURN || token.key == ROOT || token.key == ERROR_PAGE ||
        token.key == CLIENT_MAX_BODY_SIZE || token.key == INDEX || token.key == AUTOINDEX)
    {
        if (token.values[token.values.size() - 1] == OPEN_BRACKET ||
            token.values[token.values.size() - 1] == CLOSE_BRACKET)
            ;
        else if (token.values[token.values.size() - 1][token.values[token.values.size() - 1].size() - 1] == ';')
        {
            std::string tmp = token.values[token.values.size() - 1];
            token.values.pop_back();
            // ';' を探し、見つかった場合に削除
            size_t semicolonPos = tmp.find(';');
            if (semicolonPos != std::string::npos)
            {
                tmp.erase(semicolonPos, 1);
            }
            token.values.push_back(tmp);
        }
        else
        {
            utils::printError(
                std::string("token.values[token.values.size() - 1] == " + token.values[token.values.size() - 1]));
            return StringResult::Err("';' is not found in config file");
        }
    }
    return StringResult::Ok("ok");
}

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
            if (!token.values.empty())
            {
                StringResult result = removeSemicolonFromToken(token);
                if (!result.ok())
                    return TokenizeResult::Err(result.unwrapErr());
            }
            tokens.push_back(token);
        }
    }
    StringResult result = checkBracketsBalance(tokens);
    if (!result.ok())
        return TokenizeResult::Err(result.unwrapErr());
    return TokenizeResult::Ok(tokens);
}
