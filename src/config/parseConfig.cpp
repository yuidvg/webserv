#include "parseConfig.hpp"
#include "parseDirective.hpp"

using namespace parseDirective;

void removeSemicolonFromToken(std::vector<std::string> &tokens)
{
    if (tokens[0] == SERVER_NAME || tokens[0] == LISTEN || tokens[0] == ALLOW_METHOD || tokens[0] == CGI_EXECUTOR ||
        tokens[0] == UPLOAD_PATH || tokens[0] == RETURN || tokens[0] == ROOT || tokens[0] == ERROR_PAGE ||
        tokens[0] == CLIENT_MAX_BODY_SIZE || tokens[0] == INDEX || tokens[0] == AUTOINDEX)
    {
        if (tokens[tokens.size() - 1] == OPEN_BRACKET || tokens[tokens.size() - 1] == CLOSE_BRACKET)
            ;
        else if (tokens[tokens.size() - 1][tokens[tokens.size() - 1].size() - 1] == ';')
        {
            std::string tmp = tokens[tokens.size() - 1];
            tokens.pop_back();
            // ';' を探し、見つかった場合に削除
            size_t semicolonPos = tmp.find(';');
            if (semicolonPos != std::string::npos)
            {
                tmp.erase(semicolonPos, 1);
            }
            tokens.push_back(tmp);
        }
    }
}

// ロケーションブロックの設定を解析
ParseLocationResult parseLocationContext(std::vector<std::string> &tokens, std::string &locationPath)
{
    std::string path = locationPath;
    std::string root = "";
    bool autoindex = false;
    std::string index = "index.html";
    size_t clientMaxBodySize = 0;
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
        if (!directiveTokensResult.ok())
            return ParseLocationResult::Err(directiveTokensResult.unwrapErr());
        std::vector<std::string> directiveTokens = directiveTokensResult.unwrap();

        if (directiveTokens[0] == ROOT)
        {
            RootResult rootRes = parseRootDirective(directiveTokens);
            if (!rootRes.ok())
                return ParseLocationResult::Err(rootRes.unwrapErr());
            root = rootRes.unwrap();
        }
        else if (directiveTokens[0] == AUTOINDEX)
        {
            AutoindexResult autoindexRes = parseAutoindexDirective(directiveTokens);
            if (!autoindexRes.ok())
                return ParseLocationResult::Err(autoindexRes.unwrapErr());
            autoindex = autoindexRes.unwrap();
        }
        else if (directiveTokens[0] == INDEX)
        {
            IndexResult indexRes = parseIndexDirective(directiveTokens);
            if (!indexRes.ok())
                return ParseLocationResult::Err(indexRes.unwrapErr());
            index = indexRes.unwrap();
        }
        else if (directiveTokens[0] == CLIENT_MAX_BODY_SIZE)
        {
            ClientMaxBodySizeResult clientMaxBodySizeRes = parseClientMaxBodySize(directiveTokens);
            if (!clientMaxBodySizeRes.ok())
                return ParseLocationResult::Err(clientMaxBodySizeRes.unwrapErr());
            clientMaxBodySize = clientMaxBodySizeRes.unwrap();
        }
        else if (directiveTokens[0] == ERROR_PAGE)
        {
            ErrorPagesResult errorPageRes = parseErrorPage(directiveTokens);
            if (!errorPageRes.ok())
                return ParseLocationResult::Err(errorPageRes.unwrapErr());
            std::map<int, std::string> newErrorPages = errorPageRes.unwrap();
            errorPages.insert(newErrorPages.begin(), newErrorPages.end());
        }
        else if (directiveTokens[0] == ALLOW_METHOD)
        {
            AllowMethodsResult allowMethodsRes = parseAllowMethodDirective(directiveTokens);
            if (!allowMethodsRes.ok())
                return ParseLocationResult::Err(allowMethodsRes.unwrapErr());
            allowMethods = allowMethodsRes.unwrap();
        }
        else if (directiveTokens[0] == CGI_EXECUTOR)
        {
            CgiExtensionResult cgiExtensionRes = parseCgiExecutorDirective(directiveTokens);
            if (!cgiExtensionRes.ok())
                return ParseLocationResult::Err(cgiExtensionRes.unwrapErr());
            cgiExtension = cgiExtensionRes.unwrap();
        }
        else if (directiveTokens[0] == UPLOAD_PATH)
        {
            UploadPathResult uploadPathRes = parseUploadPathDirective(directiveTokens);
            if (!uploadPathRes.ok())
                return ParseLocationResult::Err(uploadPathRes.unwrapErr());
            uploadPath = uploadPathRes.unwrap();
        }
        else if (directiveTokens[0] == RETURN)
        {
            RedirectResult redirectRes = parseReturnDirective(directiveTokens);
            if (!redirectRes.ok())
            {
                return ParseLocationResult::Err(redirectRes.unwrapErr());
            }
            std::map<int, std::string> newRedirect = redirectRes.unwrap();
            redirect.insert(newRedirect.begin(), newRedirect.end());
        }

        else if (directiveTokens[0] == CLOSE_BRACKET)
        {
            return ParseLocationResult::Ok(Location(path, root, autoindex, index, clientMaxBodySize, errorPages,
                                                    allowMethods, cgiExtension, uploadPath, redirect));
        }
    }
    return ParseLocationResult::Err("Locationブロックで文字列が見つかりませんでした");
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
        if (!directiveTokensResult.ok())
            return ParseServerResult::Err(directiveTokensResult.unwrapErr());
        std::vector<std::string> directiveTokens = directiveTokensResult.unwrap();

        if (directiveTokens[0] == LOCATION)
        {
            LocationResult locationRes = parseLocationDirective(directiveTokens, tokens);
            if (!locationRes.ok())
                return ParseServerResult::Err(locationRes.unwrapErr());
            locations.push_back(locationRes.unwrap());
        }
        else if (directiveTokens[0] == SERVER_NAME)
        {
            NameResult nameRes = parseServerName(directiveTokens);
            if (!nameRes.ok())
                return ParseServerResult::Err(nameRes.unwrapErr());
            name = nameRes.unwrap();
        }
        else if (directiveTokens[0] == LISTEN)
        {
            PortResult listenRes = parseListen(directiveTokens);
            if (!listenRes.ok())
                return ParseServerResult::Err(listenRes.unwrapErr());
            port = listenRes.unwrap();
        }
        else if (directiveTokens[0] == ERROR_PAGE)
        {
            ErrorPagesResult errorPageRes = parseErrorPage(directiveTokens);
            if (!errorPageRes.ok())
                return ParseServerResult::Err(errorPageRes.unwrapErr());
            std::map<int, std::string> newErrorPages = errorPageRes.unwrap();
            errorPages.insert(newErrorPages.begin(), newErrorPages.end());
        }
        else if (directiveTokens[0] == CLIENT_MAX_BODY_SIZE)
        {
            ClientMaxBodySizeResult clientMaxBodySizeRes = parseClientMaxBodySize(directiveTokens);
            if (!clientMaxBodySizeRes.ok())
                return ParseServerResult::Err(clientMaxBodySizeRes.unwrapErr());
            clientMaxBodySize = clientMaxBodySizeRes.unwrap();
        }
        else if (directiveTokens[0] == CLOSE_BRACKET)
        {
            return ParseServerResult::Ok(Server(name, port, errorPages, clientMaxBodySize, locations));
        }
    }

    return ParseServerResult::Err(
        "Serverブロックが正常に終了しませんでした。\nserverブロックの中身が無い or } がありません");
}

// 設定ファイルを解析するメインの関数
ParseResult parseConfig(const char *configPath)
{
    TokensResult tokensResult = tokenize(configPath);
    if (!tokensResult.ok())
        return ParseResult::Err(tokensResult.unwrapErr());
    std::vector<std::string> tokens = tokensResult.unwrap();

    std::vector<Server> servers;

    while (!tokens.empty())
    {
        TokensResult directiveTokensResult = extractADirective(tokens);
        if (!directiveTokensResult.ok())
            return ParseResult::Err(directiveTokensResult.unwrapErr());
        std::vector<std::string> directiveTokens = directiveTokensResult.unwrap();

        if (directiveTokens[0] == SERVER)
        {
            ServerResult serverRes = parseServer(directiveTokens, tokens);
            if (!serverRes.ok())
                return ParseResult::Err(serverRes.unwrapErr());
            servers.push_back(serverRes.unwrap());
        }
        else
        {
            return ParseResult::Err("Config: serverブロックがありません");
        }
    }
    return ParseResult::Ok(servers);
}
