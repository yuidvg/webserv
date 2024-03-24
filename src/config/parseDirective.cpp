#include "parseDirective.hpp"
#include "../all.hpp"
#include "parseConfig.hpp"

namespace parseConfig
{

ErrorPage generateErrorPage(const int statusCode, const std::string &path)
{
    const FileContentResult fileContentResult = utils::fileContent(path);
    if (fileContentResult.success)
    {
        return ErrorPage(statusCode, HttpResponse(statusCode, fileContentResult.value, utils::contentType(path)));
    }
    if (statusCode == SERVER_ERROR)
        return ErrorPage(statusCode, SERVER_ERROR_RESPONSE);
    return ErrorPage(BAD_REQUEST, BAD_REQUEST_RESPONSE);
}

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

    StringToIntResult result = utils::stringToInt(directiveTokens[1], 0, 65535);
    if (!result.success)
    {
        return PortResult::Error(result.error);
    }

    return PortResult::Success(result.value);
}

ErrorPageResult parseErrorPage(const Strings directiveTokens)
{
    if (directiveTokens.size() != 3)
        return ErrorPageResult::Error("Config: error_pageの引数が多いです");

    StringToIntResult errorCodeResult = utils::stringToInt(directiveTokens[1], 100, 599);
    if (!errorCodeResult.success)
    {
        return ErrorPageResult::Error("Config: error_pageのエラーコードが不正です");
    }
    int errorCode = errorCodeResult.value;

    ErrorPages errorPages;
    return ErrorPageResult::Success(generateErrorPage(errorCode, directiveTokens[2]));
}

ClientMaxBodySizeResult parseClientMaxBodySize(const std::vector<std::string> directiveTokens)
{
    if (directiveTokens.size() != 2)
        return ClientMaxBodySizeResult::Error(directiveTokens[1] + "引数の数が不正です");

    StringToIntResult sizeResult = utils::stringToInt(directiveTokens[1], 0, INT_MAX);
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
    if (directiveTokens.size() != 2)
    {
        return RedirectResult::Error("Config: returnの引数が不正です");
    }
    const std::string uri = directiveTokens[1];
    return RedirectResult::Success(uri);
}

} // namespace parseConfig
