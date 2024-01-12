#include "directiveParser.hpp"

using namespace directiveParser;

stringToIntResult stringToInt(const std::string &str, int minVal, int maxVal)
{
    if (!utils::isNumber(str))
    {
        return stringToIntResult::Err("値が数値ではありません");
    }

    std::istringstream iss(str);
    int num;
    if (!(iss >> num) || !iss.eof())
    {
        return stringToIntResult::Err("数値の変換に失敗しました");
    }

    if (num < minVal || num > maxVal)
    {
        return stringToIntResult::Err("数値が許容範囲外です");
    }

    return stringToIntResult::Ok(num);
}

/*
    各ディレクティブのパーサー
*/

serverResult parseServer(Tokenize token, std::vector<Tokenize> &tokens)
{
    if (token.values.size() > 1)
        return serverResult::Err("Config: serverの引数が多いです");
    if (tokens.front().key != OPEN_BRACKET && token.values[0] != OPEN_BRACKET)
        return serverResult::Err("Config: serverブロックの開始が不正です");
    if (tokens.front().key == OPEN_BRACKET)
        tokens.erase(tokens.begin());

    ParseServerResult serverRes = ParseServer(tokens);
    if (!serverRes.ok())
        return serverResult::Err(serverRes.unwrapErr());

    return serverResult::Ok(serverRes.unwrap());
}

nameResult parseServerName(const Tokenize &token)
{
    if (token.values.size() != 1)
        return nameResult::Err("Config: server_nameの引数が多いです");
    return nameResult::Ok(token.values[0]);
}

portResult parseListen(const Tokenize &token)
{
    if (token.values.size() != 1)
        return portResult::Err("Config: listenの引数が多いです");

    stringToIntResult result = stringToInt(token.values[0], 0, 65535);
    if (!result.ok())
    {
        return portResult::Err(result.unwrapErr());
    }

    return portResult::Ok(result.unwrap());
}

errorPagesResult parseErrorPage(const Tokenize &token)
{
    if (token.values.size() != 2)
        return errorPagesResult::Err("Config: error_pageの引数が多いです");

    stringToIntResult errorCodeResult = stringToInt(token.values[0], 100, 599);
    if (!errorCodeResult.ok())
    {
        return errorPagesResult::Err("Config: error_pageのエラーコードが不正です");
    }
    int errorCode = errorCodeResult.unwrap();

    std::map<int, std::string> errorPages;
    errorPages[errorCode] = token.values[1];
    return errorPagesResult::Ok(errorPages);
}

clientMaxBodySizeResult parseClientMaxBodySize(const Tokenize &token)
{
    if (token.values.size() != 1)
        return clientMaxBodySizeResult::Err(token.key + "引数の数が不正です");

    stringToIntResult sizeResult = stringToInt(token.values[0], 0, INT_MAX);
    if (!sizeResult.ok())
    {
        return clientMaxBodySizeResult::Err("Config: client_max_body_sizeの値が不正です");
    }

    size_t size = static_cast<size_t>(sizeResult.unwrap());
    return clientMaxBodySizeResult::Ok(size);
}

locationResult parseLocationDirective(Tokenize token, std::vector<Tokenize> &tokens)
{
    if (token.values.size() > 2)
        return locationResult::Err("Config: locationの引数が多いです");
    if (tokens.front().key != OPEN_BRACKET && token.values[1] != OPEN_BRACKET)
        return locationResult::Err("Config: locationブロックの開始が不正です");

    std::string locationPath = token.values[0];
    if (tokens.front().key == OPEN_BRACKET)
    {
        tokens.erase(tokens.begin()); // OPEN_BRACKET を削除
    }

    // ParseLocation 関数を呼び出して location の設定を解析
    ParseLocationResult locationResult = ParseLocation(tokens, locationPath);
    if (!locationResult.ok())
    {
        return locationResult::Err(locationResult.unwrapErr());
    }

    // 成功した場合、解析結果を返す
    return locationResult::Ok(locationResult.unwrap());
}

/*
Location
*/
rootResult parseRootDirective(const Tokenize &token)
{
    if (token.values.size() != 1)
    {
        return rootResult::Err(token.key + "引数の数が不正です");
    }
    return rootResult::Ok(token.values[0]);
}

autoindexResult parseAutoindexDirective(const Tokenize &token)
{
    if (token.values.size() != 1)
    {
        return autoindexResult::Err(token.key + "引数の数が不正です");
    }
    bool autoindexValue = (token.values[0] == "on");
    return autoindexResult::Ok(autoindexValue);
}

indexResult parseIndexDirective(const Tokenize &token)
{
    if (token.values.size() != 1)
    {
        return indexResult::Err(token.key + "引数の数が不正です");
    }
    return indexResult::Ok(token.values[0]);
}

// location専用
allowMethodsResult parseAllowMethodDirective(const Tokenize &token)
{
    if (token.values.size() > 3 || token.values.size() < 1)
    {
        return allowMethodsResult::Err("Config: allow_methodの引数が不正です");
    }
    std::vector<std::string> methods;
    for (size_t i = 0; i < token.values.size(); ++i)
    {
        std::string method = token.values[i];
        if (method != "GET" && method != "POST" && method != "DELETE")
        {
            return allowMethodsResult::Err("Config: 許可されないHTTPメソッドが指定されています");
        }
        methods.push_back(method);
    }
    return allowMethodsResult::Ok(methods);
}

cgiExtensionResult parseCgiExecutorDirective(const Tokenize &token)
{
    if (token.values.size() != 1)
    {
        return cgiExtensionResult::Err("Config: cgi_executorの引数が多いです");
    }
    return cgiExtensionResult::Ok(token.values[0]);
}

uploadPathResult parseUploadPathDirective(const Tokenize &token)
{
    if (token.values.size() != 1)
    {
        return uploadPathResult::Err(token.key + "引数の数が不正です");
    }
    return uploadPathResult::Ok(token.values[0]);
}

redirectResult parseReturnDirective(const Tokenize &token)
{
    if (token.values.size() != 2)
    {
        return redirectResult::Err("Config: returnの引数が不正です");
    }

    stringToIntResult statusCodeResult = stringToInt(token.values[0], 300, 399);
    if (!statusCodeResult.ok())
        return redirectResult::Err("Config: returnのステータスコードが不正です");
    std::map<int, std::string> redirect;
    redirect[statusCodeResult.unwrap()] = token.values[1];
    return redirectResult::Ok(redirect);
}
