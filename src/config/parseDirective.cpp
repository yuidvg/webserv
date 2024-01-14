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

ServerResult parseServer(const std::vector<std::string> directiveTokens, std::vector<std::string> &tokens)
{
    if (directiveTokens.size() != 2)
        return ServerResult::Err("Config: serverの引数が多いです");
    if (directiveTokens[1] != OPEN_BRACKET)
        return ServerResult::Err("Config: serverブロックの開始が不正です");

    ParseServerResult serverRes = parseServerContext(tokens);
    if (!serverRes.ok())
        return ServerResult::Err(serverRes.unwrapErr());

    return ServerResult::Ok(serverRes.unwrap());
}

parseDirective::NameResult parseServerName(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
        return NameResult::Err("Config: server_nameの引数が多いです");
    return NameResult::Ok(directiveTokens[1]);
}

parseDirective::PortResult parseListen(const std::vector<std::string> directiveTokens)
{

    if (directiveTokens.size() != 2)
        return PortResult::Err("Config: listenの引数が多いです");

    StringToIntResult result = stringToInt(directiveTokens[1], 0, 65535);
    if (!result.ok())
    {
        return PortResult::Err(result.unwrapErr());
    }

    return PortResult::Ok(result.unwrap());
}

ErrorPagesResult parseErrorPage(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 3)
        return ErrorPagesResult::Err("Config: error_pageの引数が多いです");

    StringToIntResult errorCodeResult = stringToInt(directiveTokens[1], 100, 599);
    if (!errorCodeResult.ok())
    {
        return ErrorPagesResult::Err("Config: error_pageのエラーコードが不正です");
    }
    int errorCode = errorCodeResult.unwrap();

    std::map<int, std::string> errorPages;
    errorPages[errorCode] = directiveTokens[2];
    return ErrorPagesResult::Ok(errorPages);
}

ClientMaxBodySizeResult parseClientMaxBodySize(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
        return ClientMaxBodySizeResult::Err(directiveTokens[1] + "引数の数が不正です");

    StringToIntResult sizeResult = stringToInt(directiveTokens[1], 0, INT_MAX);
    if (!sizeResult.ok())
    {
        return ClientMaxBodySizeResult::Err("Config: client_max_body_sizeの値が不正です");
    }

    size_t size = static_cast<size_t>(sizeResult.unwrap());
    return ClientMaxBodySizeResult::Ok(size);
}

LocationResult parseLocationDirective(const std::vector<std::string> directiveTokens, std::vector<std::string> &tokens)
{
    if (directiveTokens.size() != 3)
        return LocationResult::Err("Config: locationの引数が不正です");
    if (directiveTokens[2] != OPEN_BRACKET)
        return LocationResult::Err("Config: locationブロックの開始が不正です");

    std::string locationPath = directiveTokens[1];

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
RootResult parseRootDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
    {
        return RootResult::Err(directiveTokens[1] + "引数の数が不正です");
    }
    return RootResult::Ok(directiveTokens[1]);
}

AutoindexResult parseAutoindexDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
    {
        return AutoindexResult::Err(directiveTokens[1] + "引数の数が不正です");
    }
    bool autoindexValue = (directiveTokens[1] == "on");
    return AutoindexResult::Ok(autoindexValue);
}

IndexResult parseIndexDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
    {
        return IndexResult::Err(directiveTokens[1] + "引数の数が不正です");
    }
    return IndexResult::Ok(directiveTokens[1]);
}

// location専用
AllowMethodsResult parseAllowMethodDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() > 4 || directiveTokens.size() < 2)
    {
        return AllowMethodsResult::Err("Config: allow_methodの引数が不正です");
    }
    std::vector<std::string> methods;
    for (size_t i = 1; i < directiveTokens.size(); ++i)
    {
        std::string method = directiveTokens[i];
        if (method != "GET" && method != "POST" && method != "DELETE")
        {
            return AllowMethodsResult::Err("Config: 許可されないHTTPメソッドが指定されています");
        }
        methods.push_back(method);
    }
    return AllowMethodsResult::Ok(methods);
}

CgiExtensionResult parseCgiExecutorDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
    {
        return CgiExtensionResult::Err("Config: cgi_executorの引数が多いです");
    }
    return CgiExtensionResult::Ok(directiveTokens[1]);
}

UploadPathResult parseUploadPathDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
    {
        return UploadPathResult::Err(directiveTokens[1] + "引数の数が不正です");
    }
    return UploadPathResult::Ok(directiveTokens[1]);
}

RedirectResult parseReturnDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 3)
    {
        return RedirectResult::Err("Config: returnの引数が不正です");
    }

    StringToIntResult statusCodeResult = stringToInt(directiveTokens[1], 300, 399);
    if (!statusCodeResult.ok())
        return RedirectResult::Err("Config: returnのステータスコードが不正です");
    std::map<int, std::string> redirect;
    redirect[statusCodeResult.unwrap()] = directiveTokens[2];
    return RedirectResult::Ok(redirect);
}
