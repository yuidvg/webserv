#include "parseConfig.hpp"
#include "parseDirective.hpp"

using namespace parseDirective;

// ロケーションブロックの設定を解析
ParseLocationResult parseLocationContext(std::vector<std::string> &tokens, std::string &locationPath)
{
    std::string path = locationPath;
    std::string root = "";
    bool autoindex = false;
    std::string index = "index.html";
    size_t clientMaxBodySize = 1048576;
    std::map<int, std::string> errorPages;
    std::vector<std::string> allowMethods;
    allowMethods.push_back("GET");
    allowMethods.push_back("POST");
    allowMethods.push_back("DELETE");
    std::string cgiExtension = "";
    std::string uploadPath = "";
    std::map<int, std::string> redirect;

    while (!tokens.empty())
    {
        TokensResult directiveTokensResult = extractADirective(tokens);
        if (!directiveTokensResult.success)
            return ParseLocationResult::Error(directiveTokensResult.error);
        std::vector<std::string> directiveTokens = directiveTokensResult.value;

        if (directiveTokens[0] == ROOT)
        {
            RootResult rootRes = parseRootDirective(directiveTokens);
            if (!rootRes.success)
                return ParseLocationResult::Error(rootRes.error);
            root = rootRes.value;
        }
        else if (directiveTokens[0] == AUTOINDEX)
        {
            AutoindexResult autoindexRes = parseAutoindexDirective(directiveTokens);
            if (!autoindexRes.success)
                return ParseLocationResult::Error(autoindexRes.error);
            autoindex = autoindexRes.value;
        }
        else if (directiveTokens[0] == INDEX)
        {
            IndexResult indexRes = parseIndexDirective(directiveTokens);
            if (!indexRes.success)
                return ParseLocationResult::Error(indexRes.error);
            index = indexRes.value;
        }
        else if (directiveTokens[0] == CLIENT_MAX_BODY_SIZE)
        {
            ClientMaxBodySizeResult clientMaxBodySizeRes = parseClientMaxBodySize(directiveTokens);
            if (!clientMaxBodySizeRes.success)
                return ParseLocationResult::Error(clientMaxBodySizeRes.error);
            clientMaxBodySize = clientMaxBodySizeRes.value;
        }
        else if (directiveTokens[0] == ERROR_PAGE)
        {
            ErrorPagesResult errorPageRes = parseErrorPage(directiveTokens);
            if (!errorPageRes.success)
                return ParseLocationResult::Error(errorPageRes.error);
            std::map<int, std::string> newErrorPages = errorPageRes.value;
            errorPages.insert(newErrorPages.begin(), newErrorPages.end());
        }
        else if (directiveTokens[0] == ALLOW_METHOD)
        {
            AllowMethodsResult allowMethodsRes = parseAllowMethodDirective(directiveTokens);
            if (!allowMethodsRes.success)
                return ParseLocationResult::Error(allowMethodsRes.error);
            allowMethods = allowMethodsRes.value;
        }
        else if (directiveTokens[0] == CGI_EXECUTOR)
        {
            CgiExtensionResult cgiExtensionRes = parseCgiExecutorDirective(directiveTokens);
            if (!cgiExtensionRes.success)
                return ParseLocationResult::Error(cgiExtensionRes.error);
            cgiExtension = cgiExtensionRes.value;
        }
        else if (directiveTokens[0] == UPLOAD_PATH)
        {
            UploadPathResult uploadPathRes = parseUploadPathDirective(directiveTokens);
            if (!uploadPathRes.success)
                return ParseLocationResult::Error(uploadPathRes.error);
            uploadPath = uploadPathRes.value;
        }
        else if (directiveTokens[0] == RETURN)
        {
            RedirectResult redirectRes = parseReturnDirective(directiveTokens);
            if (!redirectRes.success)
            {
                return ParseLocationResult::Error(redirectRes.error);
            }
            std::map<int, std::string> newRedirect = redirectRes.value;
            redirect.insert(newRedirect.begin(), newRedirect.end());
        }

        else if (directiveTokens[0] == CLOSE_BRACKET)
        {
            return ParseLocationResult::Success(Location(path, root, autoindex, index, clientMaxBodySize, errorPages,
                                                    allowMethods, cgiExtension, uploadPath, redirect));
        }
    }
    return ParseLocationResult::Error("Locationブロックで文字列が見つかりませんでした");
}

// サーバーブロックの設定を解析
ParseServerResult parseServerContext(std::vector<std::string> &tokens)
{
    std::string name = "";
    size_t port = 80;
    std::map<int, std::string> errorPages;
    size_t clientMaxBodySize = 1048576;
    std::vector<Location> locations;

    while (!tokens.empty())
    {
        TokensResult directiveTokensResult = extractADirective(tokens);
        if (!directiveTokensResult.success)
            return ParseServerResult::Error(directiveTokensResult.error);
        std::vector<std::string> directiveTokens = directiveTokensResult.value;

        if (directiveTokens[0] == LOCATION)
        {
            LocationResult locationRes = parseLocationDirective(directiveTokens, tokens);
            if (!locationRes.success)
                return ParseServerResult::Error(locationRes.error);
            locations.push_back(locationRes.value);
        }
        else if (directiveTokens[0] == SERVER_NAME)
        {
            NameResult nameRes = parseServerName(directiveTokens);
            if (!nameRes.success)
                return ParseServerResult::Error(nameRes.error);
            name = nameRes.value;
        }
        else if (directiveTokens[0] == LISTEN)
        {
            PortResult listenRes = parseListen(directiveTokens);
            if (!listenRes.success)
                return ParseServerResult::Error(listenRes.error);
            port = listenRes.value;
        }
        else if (directiveTokens[0] == ERROR_PAGE)
        {
            ErrorPagesResult errorPageRes = parseErrorPage(directiveTokens);
            if (!errorPageRes.success)
                return ParseServerResult::Error(errorPageRes.error);
            std::map<int, std::string> newErrorPages = errorPageRes.value;
            errorPages.insert(newErrorPages.begin(), newErrorPages.end());
        }
        else if (directiveTokens[0] == CLIENT_MAX_BODY_SIZE)
        {
            ClientMaxBodySizeResult clientMaxBodySizeRes = parseClientMaxBodySize(directiveTokens);
            if (!clientMaxBodySizeRes.success)
                return ParseServerResult::Error(clientMaxBodySizeRes.error);
            clientMaxBodySize = clientMaxBodySizeRes.value;
        }
        else if (directiveTokens[0] == CLOSE_BRACKET)
        {
            return ParseServerResult::Success(Server(name, port, errorPages, clientMaxBodySize, locations));
        }
    }

    return ParseServerResult::Error(
        "Serverブロックが正常に終了しませんでした。\nserverブロックの中身が無い or } がありません");
}

// 設定ファイルを解析するメインの関数
ParseResult parseConfig(const char *configPath)
{
    TokensResult tokensResult = tokenize(configPath);
    if (!tokensResult.success)
        return ParseResult::Error(tokensResult.error);
    std::vector<std::string> tokens = tokensResult.value;

    std::vector<Server> servers;

    while (!tokens.empty())
    {
        TokensResult directiveTokensResult = extractADirective(tokens);
        if (!directiveTokensResult.success)
            return ParseResult::Error(directiveTokensResult.error);
        std::vector<std::string> directiveTokens = directiveTokensResult.value;

        if (directiveTokens[0] == SERVER)
        {
            ServerResult serverRes = parseServer(directiveTokens, tokens);
            if (!serverRes.success)
                return ParseResult::Error(serverRes.error);
            servers.push_back(serverRes.value);
        }
        else
        {
            return ParseResult::Error("Config: serverブロックがありません");
        }
    }
    return ParseResult::Success(servers);
}
