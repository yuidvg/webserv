#include "tokenizeConfig.hpp"

stringResult checkBracketsBalance(const std::vector<Tokenize> &tokens)
{
    int brackets = 0;
    for (std::vector<Tokenize>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        if (it->key == OPEN_BRACKET)
        {
            brackets++;
        }
        else if (it->key == CLOSE_BRACKET)
        {
            brackets--;
        }

        for (std::vector<std::string>::const_iterator valueIt = it->values.begin(); valueIt != it->values.end();
             ++valueIt)
        {
            if (*valueIt == OPEN_BRACKET)
            {
                brackets++;
            }
            else if (*valueIt == CLOSE_BRACKET)
            {
                brackets--;
            }
        }

        // サーバーコンテキスト内での2回ネストされたコンテキストは許可されません
        if (brackets > 2)
        {
            return stringResult::Err("Too many nested contexts in config file");
        }
    }

    if (brackets != 0)
    {
        return stringResult::Err("Invalid number of brackets " + utils::to_string(brackets) + " in config file");
    }

    return stringResult::Ok("ok");
}

tokenizeResult tokenize(const char *configPath)
{
    std::ifstream configFile(configPath);
    std::string line;
    std::vector<Tokenize> tokens;
    // TODO: ;のエラーチェック
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
            // 末尾の';'を削除
            if (token.key == SERVER || token.key == SERVER_NAME || token.key == LISTEN || token.key == LOCATION ||
                token.key == ALLOW_METHOD || token.key == CGI_EXECUTOR || token.key == UPLOAD_PATH ||
                token.key == RETURN || token.key == ROOT || token.key == ERROR_PAGE ||
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
                    utils::printError(std::string("token.values[token.values.size() - 1] == " +
                                                  token.values[token.values.size() - 1]));
                    return tokenizeResult::Err("';' is not found in config file");
                }
            }

            tokens.push_back(token);
        }
    }
    // ブラケットのバランスチェック
    int brackets = 0;
    for (std::vector<Tokenize>::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        if (!(it->key == SERVER || it->key == SERVER_NAME || it->key == LISTEN || it->key == LOCATION ||
              it->key == ALLOW_METHOD || it->key == CGI_EXECUTOR || it->key == UPLOAD_PATH || it->key == RETURN ||
              it->key == ROOT || it->key == ERROR_PAGE || it->key == CLIENT_MAX_BODY_SIZE || it->key == INDEX ||
              it->key == AUTOINDEX || it->key == OPEN_BRACKET || it->key == CLOSE_BRACKET))
        {
            return tokenizeResult::Err("Invalid key = " + it->key + " in config file");
            // エラーハンドリングのコードをここに挿入
        }
        if (it->key == OPEN_BRACKET)
            brackets++;
        else if (it->key == CLOSE_BRACKET)
            brackets--;
        for (std::vector<std::string>::iterator valueIt = it->values.begin(); valueIt != it->values.end(); ++valueIt)
        {
            if (*valueIt == OPEN_BRACKET)
                brackets++;
            else if (*valueIt == CLOSE_BRACKET)
                brackets--;
        }
        // サーバーコンテキスト内での2回ネストされたコンテキストは許可されません
        if (brackets > 2)
        {
            return tokenizeResult::Err("Too many nested contexts in config file");
        }
    }
    if (brackets != 0)
    {
        return tokenizeResult::Err("Invalid number of brackets " + utils::to_string(brackets) + " in config file");
    }
    return tokenizeResult::Ok(tokens);
}
