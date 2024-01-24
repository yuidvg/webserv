#include "parseConfig.hpp"
#include "parseDirective.hpp"

namespace parseConfig
{

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

/*
    Server directive
*/

ServerResult parseServer(const std::vector<std::string> directiveTokens, std::vector<std::string> &tokens)
{
    if (directiveTokens.size() != 2)
        return ServerResult::Error("Config: serverの引数が多いです");
    if (directiveTokens[1] != OPEN_BRACKET)
        return ServerResult::Error("Config: serverブロックの開始が不正です");

    ParseServerResult serverRes = parseServerContext(tokens);
    if (!serverRes.success)
        return ServerResult::Error(serverRes.error);

    return ServerResult::Success(serverRes.value);
}

NameResult parseServerName(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
        return NameResult::Error("Config: server_nameの引数が多いです");
    return NameResult::Success(directiveTokens[1]);
}

PortResult parseListen(const std::vector<std::string> directiveTokens)
{

    if (directiveTokens.size() != 2)
        return PortResult::Error("Config: listenの引数が多いです");

    StringToIntResult result = stringToInt(directiveTokens[1], 0, 65535);
    if (!result.success)
    {
        return PortResult::Error(result.error);
    }

    return PortResult::Success(result.value);
}

ErrorPagesResult parseErrorPage(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 3)
        return ErrorPagesResult::Error("Config: error_pageの引数が多いです");

    StringToIntResult errorCodeResult = stringToInt(directiveTokens[1], 100, 599);
    if (!errorCodeResult.success)
    {
        return ErrorPagesResult::Error("Config: error_pageのエラーコードが不正です");
    }
    int errorCode = errorCodeResult.value;

    std::map<int, std::string> errorPages;
    errorPages[errorCode] = directiveTokens[2];
    return ErrorPagesResult::Success(errorPages);
}

ClientMaxBodySizeResult parseClientMaxBodySize(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
        return ClientMaxBodySizeResult::Error(directiveTokens[1] + "引数の数が不正です");

    StringToIntResult sizeResult = stringToInt(directiveTokens[1], 0, INT_MAX);
    if (!sizeResult.success)
    {
        return ClientMaxBodySizeResult::Error("Config: client_max_body_sizeの値が不正です");
    }

    size_t size = static_cast<size_t>(sizeResult.value);
    return ClientMaxBodySizeResult::Success(size);
}

LocationResult parseLocationDirective(const std::vector<std::string> directiveTokens, std::vector<std::string> &tokens)
{
    if (directiveTokens.size() != 3)
        return LocationResult::Error("Config: locationの引数が不正です");
    if (directiveTokens[2] != OPEN_BRACKET)
        return LocationResult::Error("Config: locationブロックの開始が不正です");

    std::string locationPath = directiveTokens[1];

    ParseLocationResult locationResult = parseLocationContext(tokens, locationPath);
    if (!locationResult.success)
    {
        return LocationResult::Error(locationResult.error);
    }

    return LocationResult::Success(locationResult.value);
}

/*
location directive
*/
RootResult parseRootDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
    {
        return RootResult::Error(directiveTokens[1] + "引数の数が不正です");
    }
    return RootResult::Success(directiveTokens[1]);
}

AutoindexResult parseAutoindexDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
    {
        return AutoindexResult::Error(directiveTokens[1] + "引数の数が不正です");
    }
    bool autoindexValue = (directiveTokens[1] == "on");
    return AutoindexResult::Success(autoindexValue);
}

IndexResult parseIndexDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
    {
        return IndexResult::Error(directiveTokens[1] + "引数の数が不正です");
    }
    return IndexResult::Success(directiveTokens[1]);
}

AllowMethodsResult parseAllowMethodDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() > 4 || directiveTokens.size() < 2)
    {
        return AllowMethodsResult::Error("Config: allow_methodの引数が不正です");
    }
    std::vector<std::string> methods;
    for (size_t i = 1; i < directiveTokens.size(); ++i)
    {
        std::string method = directiveTokens[i];
        if (method != "GET" && method != "POST" && method != "DELETE")
        {
            return AllowMethodsResult::Error("Config: 許可されないHTTPメソッドが指定されています");
        }
        methods.push_back(method);
    }
    return AllowMethodsResult::Success(methods);
}

CgiExtensionResult parseCgiExecutorDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
    {
        return CgiExtensionResult::Error("Config: cgi_extensionの引数が多いです");
    }
    return CgiExtensionResult::Success(directiveTokens[1]);
}

UploadPathResult parseUploadPathDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
    {
        return UploadPathResult::Error(directiveTokens[1] + "引数の数が不正です");
    }
    return UploadPathResult::Success(directiveTokens[1]);
}

RedirectResult parseReturnDirective(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 3)
    {
        return RedirectResult::Error("Config: returnの引数が不正です");
    }

    StringToIntResult statusCodeResult = stringToInt(directiveTokens[1], 300, 399);
    if (!statusCodeResult.success)
        return RedirectResult::Error("Config: returnのステータスコードが不正です");
    std::map<int, std::string> redirect;
    redirect[statusCodeResult.value] = directiveTokens[2];
    return RedirectResult::Success(redirect);
}

}