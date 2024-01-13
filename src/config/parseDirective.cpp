#include "parseDirective.hpp"

using namespace parseDirective;

StringToIntResult stringToInt(const std::string &str, int minVal, int maxVal)
{
    if (!utils::isNumber(str))
    {
        return StringToIntResult::Err("値が数値ではありません");
    }

    std::istringstream iss(str);
    int num;
    if (!(iss >> num) || !iss.eof())
    {
        return StringToIntResult::Err("数値の変換に失敗しました");
    }

    if (num < minVal || num > maxVal)
    {
        return StringToIntResult::Err("数値が許容範囲外です");
    }

    return StringToIntResult::Ok(num);
}

/*
    各ディレクティブのパーサー
*/

ServerResult parseServer(Tokenize token, std::vector<Tokenize> &tokens)
{
    if (token.values.size() > 1)
        return ServerResult::Err("Config: serverの引数が多いです");
    if (tokens.front().key != OPEN_BRACKET && token.values[0] != OPEN_BRACKET)
        return ServerResult::Err("Config: serverブロックの開始が不正です");
    if (tokens.front().key == OPEN_BRACKET)
        tokens.erase(tokens.begin());

    ParseServerResult serverRes = parseServerContext(tokens);
    if (!serverRes.ok())
        return ServerResult::Err(serverRes.unwrapErr());

    return ServerResult::Ok(serverRes.unwrap());
}

NameResult parseServerName(const Tokenize &token)
{
    if (token.values.size() != 1)
        return NameResult::Err("Config: server_nameの引数が多いです");
    return NameResult::Ok(token.values[0]);
}

PortResult parseListen(const Tokenize &token)
{
    if (token.values.size() != 1)
        return PortResult::Err("Config: listenの引数が多いです");

    StringToIntResult result = stringToInt(token.values[0], 0, 65535);
    if (!result.ok())
    {
        return PortResult::Err(result.unwrapErr());
    }

    return PortResult::Ok(result.unwrap());
}

ErrorPagesResult parseErrorPage(const Tokenize &token)
{
    if (token.values.size() != 2)
        return ErrorPagesResult::Err("Config: error_pageの引数が多いです");

    StringToIntResult errorCodeResult = stringToInt(token.values[0], 100, 599);
    if (!errorCodeResult.ok())
    {
        return ErrorPagesResult::Err("Config: error_pageのエラーコードが不正です");
    }
    int errorCode = errorCodeResult.unwrap();

    std::map<int, std::string> errorPages;
    errorPages[errorCode] = token.values[1];
    return ErrorPagesResult::Ok(errorPages);
}

ClientMaxBodySizeResult parseClientMaxBodySize(const Tokenize &token)
{
    if (token.values.size() != 1)
        return ClientMaxBodySizeResult::Err(token.key + "引数の数が不正です");

    StringToIntResult sizeResult = stringToInt(token.values[0], 0, INT_MAX);
    if (!sizeResult.ok())
    {
        return ClientMaxBodySizeResult::Err("Config: client_max_body_sizeの値が不正です");
    }

    size_t size = static_cast<size_t>(sizeResult.unwrap());
    return ClientMaxBodySizeResult::Ok(size);
}

LocationResult parseLocationDirective(Tokenize token, std::vector<Tokenize> &tokens)
{
    if (token.values.size() > 2)
        return LocationResult::Err("Config: locationの引数が多いです");
    if (tokens.front().key != OPEN_BRACKET && token.values[1] != OPEN_BRACKET)
        return LocationResult::Err("Config: locationブロックの開始が不正です");

    std::string locationPath = token.values[0];
    if (tokens.front().key == OPEN_BRACKET)
    {
        tokens.erase(tokens.begin()); // OPEN_BRACKET を削除
    }

    // ParseLocation 関数を呼び出して location の設定を解析
    ParseLocationResult locationResult = parseLocationContext(tokens, locationPath);
    if (!locationResult.ok())
    {
        return LocationResult::Err(locationResult.unwrapErr());
    }

    // 成功した場合、解析結果を返す
    return LocationResult::Ok(locationResult.unwrap());
}

/*
Location
*/
RootResult parseRootDirective(const Tokenize &token)
{
    if (token.values.size() != 1)
    {
        return RootResult::Err(token.key + "引数の数が不正です");
    }
    return RootResult::Ok(token.values[0]);
}

AutoindexResult parseAutoindexDirective(const Tokenize &token)
{
    if (token.values.size() != 1)
    {
        return AutoindexResult::Err(token.key + "引数の数が不正です");
    }
    bool autoindexValue = (token.values[0] == "on");
    return AutoindexResult::Ok(autoindexValue);
}

IndexResult parseIndexDirective(const Tokenize &token)
{
    if (token.values.size() != 1)
    {
        return IndexResult::Err(token.key + "引数の数が不正です");
    }
    return IndexResult::Ok(token.values[0]);
}

// location専用
AllowMethodsResult parseAllowMethodDirective(const Tokenize &token)
{
    if (token.values.size() > 3 || token.values.size() < 1)
    {
        return AllowMethodsResult::Err("Config: allow_methodの引数が不正です");
    }
    std::vector<std::string> methods;
    for (size_t i = 0; i < token.values.size(); ++i)
    {
        std::string method = token.values[i];
        if (method != "GET" && method != "POST" && method != "DELETE")
        {
            return AllowMethodsResult::Err("Config: 許可されないHTTPメソッドが指定されています");
        }
        methods.push_back(method);
    }
    return AllowMethodsResult::Ok(methods);
}

CgiExtensionResult parseCgiExecutorDirective(const Tokenize &token)
{
    if (token.values.size() != 1)
    {
        return CgiExtensionResult::Err("Config: cgi_executorの引数が多いです");
    }
    return CgiExtensionResult::Ok(token.values[0]);
}

UploadPathResult parseUploadPathDirective(const Tokenize &token)
{
    if (token.values.size() != 1)
    {
        return UploadPathResult::Err(token.key + "引数の数が不正です");
    }
    return UploadPathResult::Ok(token.values[0]);
}

RedirectResult parseReturnDirective(const Tokenize &token)
{
    if (token.values.size() != 2)
    {
        return RedirectResult::Err("Config: returnの引数が不正です");
    }

    StringToIntResult statusCodeResult = stringToInt(token.values[0], 300, 399);
    if (!statusCodeResult.ok())
        return RedirectResult::Err("Config: returnのステータスコードが不正です");
    std::map<int, std::string> redirect;
    redirect[statusCodeResult.unwrap()] = token.values[1];
    return RedirectResult::Ok(redirect);
}
