#include "parseConfig.hpp"
#include "parseDirective.hpp"
#include "tokenizeConfig.hpp"

namespace parseConfig
{

ParseLocationResult parseLocationContext(std::vector<std::string> &tokens, std::string &locationPath)
{
    Location location;
    std::string path = locationPath;
    std::string root = location.root;
    bool autoindex = location.autoindex;
    std::string index = location.index;
    std::vector<std::string> allowMethods = location.allowMethods;
    std::string cgiExtension = location.cgiExtension;
    std::string uploadPath = location.uploadPath;
    std::map<int, std::string> redirect = location.redirect;

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
        else if (directiveTokens[0] == ALLOW_METHOD)
        {
            AllowMethodsResult allowMethodsRes = parseAllowMethodDirective(directiveTokens);
            if (!allowMethodsRes.success)
                return ParseLocationResult::Error(allowMethodsRes.error);
            allowMethods = allowMethodsRes.value;
        }
        else if (directiveTokens[0] == CGI_EXTENSION)
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
            for (std::map<int, std::string>::iterator it = newRedirect.begin(); it != newRedirect.end(); ++it)
            {
                redirect.insert(*it);
            }
        }

        else if (directiveTokens[0] == CLOSE_BRACKET)
        {
            return ParseLocationResult::Success(Location(path, root, autoindex, index,
                                                         allowMethods, cgiExtension, uploadPath, redirect));
        }
        else
        {
            return ParseLocationResult::Error(std::string("Locationコンテキストの[" + directiveTokens[0] + "]が不正です"));
        }
    }
    return ParseLocationResult::Error("Locationブロックで文字列が見つかりませんでした");
}

ParseServerResult parseServerContext(std::vector<std::string> &tokens)
{
    Server server;
    std::string name = server.name;
    size_t port = server.port;
    std::map<int, std::string> errorPages;
    size_t clientMaxBodySize = server.clientMaxBodySize;
    std::vector<const Location> locations;

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
            for (std::map<int, std::string>::iterator it = newErrorPages.begin(); it != newErrorPages.end(); ++it)
            {
                errorPages.insert(*it);
            }

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
        else
        {
            return ParseServerResult::Error(std::string("Serverコンテキストの[" + directiveTokens[0] + "]が不正です"));
        }
    }

    return ParseServerResult::Error(
        "Serverブロックが正常に終了しませんでした。\nserverブロックの中身が無い or } がありません");
}


ConfigResult parseConfig(const char *configPath)
{
    TokensResult tokensResult = tokenize(configPath);
    if (!tokensResult.success)
        return ConfigResult::Error(tokensResult.error);
    std::vector<std::string> tokens = tokensResult.value;

    std::vector<const Server> servers;

    while (!tokens.empty())
    {
        TokensResult directiveTokensResult = extractADirective(tokens);
        if (!directiveTokensResult.success)
            return ConfigResult::Error(directiveTokensResult.error);
        std::vector<std::string> directiveTokens = directiveTokensResult.value;

        if (directiveTokens[0] == SERVER)
        {
            ServerResult serverRes = parseServer(directiveTokens, tokens);
            if (!serverRes.success)
                return ConfigResult::Error(serverRes.error);
            servers.push_back(serverRes.value);
        }
        else
        {
            return ConfigResult::Error("Config: serverブロックがありません");
        }
    }
    return ConfigResult::Success(servers);
}
}