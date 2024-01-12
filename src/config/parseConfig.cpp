#include "parseConfig.hpp"
#include "directiveParser.hpp"

using namespace directiveParser;

// ロケーションブロックの設定を解析
ParseLocationResult ParseLocation(std::vector<Tokenize> &tokens, std::string &locationPath)
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
        Tokenize token = tokens.front();
        tokens.erase(tokens.begin());
        if (token.key == ROOT)
        {
            rootResult rootRes = parseRootDirective(token);
            if (!rootRes.ok())
                return ParseLocationResult::Err(rootRes.unwrapErr());
            root = rootRes.unwrap();
        }
        else if (token.key == AUTOINDEX)
        {
            autoindexResult autoindexRes = parseAutoindexDirective(token);
            if (!autoindexRes.ok())
                return ParseLocationResult::Err(autoindexRes.unwrapErr());
            autoindex = autoindexRes.unwrap();
        }
        else if (token.key == INDEX)
        {
            indexResult indexRes = parseIndexDirective(token);
            if (!indexRes.ok())
                return ParseLocationResult::Err(indexRes.unwrapErr());
            index = indexRes.unwrap();
        }
        else if (token.key == CLIENT_MAX_BODY_SIZE)
        {
            clientMaxBodySizeResult clientMaxBodySizeRes = parseClientMaxBodySize(token);
            if (!clientMaxBodySizeRes.ok())
                return ParseLocationResult::Err(clientMaxBodySizeRes.unwrapErr());
            clientMaxBodySize = clientMaxBodySizeRes.unwrap();
        }
        else if (token.key == ERROR_PAGE)
        {
            errorPagesResult errorPageRes = parseErrorPage(token);
            if (!errorPageRes.ok())
                return ParseLocationResult::Err(errorPageRes.unwrapErr());
            std::map<int, std::string> newErrorPages = errorPageRes.unwrap();
            errorPages.insert(newErrorPages.begin(), newErrorPages.end());
        }
        else if (token.key == ALLOW_METHOD)
        {
            allowMethodsResult allowMethodsRes = parseAllowMethodDirective(token);
            if (!allowMethodsRes.ok())
                return ParseLocationResult::Err(allowMethodsRes.unwrapErr());
            allowMethods = allowMethodsRes.unwrap();
        }
        else if (token.key == CGI_EXECUTOR)
        {
            cgiExtensionResult cgiExtensionRes = parseCgiExecutorDirective(token);
            if (!cgiExtensionRes.ok())
                return ParseLocationResult::Err(cgiExtensionRes.unwrapErr());
            cgiExtension = cgiExtensionRes.unwrap();
        }
        else if (token.key == UPLOAD_PATH)
        {
            uploadPathResult uploadPathRes = parseUploadPathDirective(token);
            if (!uploadPathRes.ok())
                return ParseLocationResult::Err(uploadPathRes.unwrapErr());
            uploadPath = uploadPathRes.unwrap();
        }
        else if (token.key == RETURN)
        {
            redirectResult redirectRes = parseReturnDirective(token);
            if (!redirectRes.ok())
            {
                return ParseLocationResult::Err(redirectRes.unwrapErr());
            }
            std::map<int, std::string> newRedirect = redirectRes.unwrap();
            redirect.insert(newRedirect.begin(), newRedirect.end());
        }

        else if (token.key == CLOSE_BRACKET)
        {
            return ParseLocationResult::Ok(Location(path, root, autoindex, index, clientMaxBodySize, errorPages,
                                                    allowMethods, cgiExtension, uploadPath, redirect));
        }
    }
    return ParseLocationResult::Err("Locationブロックで文字列が見つかりませんでした");
}

// サーバーブロックの設定を解析
ParseServerResult ParseServer(std::vector<Tokenize> &tokens)
{
    std::string name = "";
    size_t port = 80;
    std::map<int, std::string> errorPages;
    size_t clientMaxBodySize = 1048576;
    std::vector<Location> locations;

    while (!tokens.empty())
    {
        Tokenize token = tokens.front();
        tokens.erase(tokens.begin());
        if (token.key == LOCATION)
        {
            locationResult locationRes = parseLocationDirective(token, tokens);
            if (!locationRes.ok())
                return ParseServerResult::Err(locationRes.unwrapErr());
            locations.push_back(locationRes.unwrap());
        }
        else if (token.key == SERVER_NAME)
        {
            nameResult nameRes = parseServerName(token);
            if (!nameRes.ok())
                return ParseServerResult::Err(nameRes.unwrapErr());
            name = nameRes.unwrap();
        }
        else if (token.key == LISTEN)
        {
            portResult listenRes = parseListen(token);
            if (!listenRes.ok())
                return ParseServerResult::Err(listenRes.unwrapErr());
            port = listenRes.unwrap();
        }
        else if (token.key == ERROR_PAGE)
        {
            errorPagesResult errorPageRes = parseErrorPage(token);
            if (!errorPageRes.ok())
                return ParseServerResult::Err(errorPageRes.unwrapErr());
            std::map<int, std::string> newErrorPages = errorPageRes.unwrap();
            errorPages.insert(newErrorPages.begin(), newErrorPages.end());
        }
        else if (token.key == CLIENT_MAX_BODY_SIZE)
        {
            clientMaxBodySizeResult clientMaxBodySizeRes = parseClientMaxBodySize(token);
            if (!clientMaxBodySizeRes.ok())
                return ParseServerResult::Err(clientMaxBodySizeRes.unwrapErr());
            clientMaxBodySize = clientMaxBodySizeRes.unwrap();
        }
        else if (token.key == CLOSE_BRACKET)
        {
            return ParseServerResult::Ok(Server(name, port, errorPages, clientMaxBodySize, locations));
        }
    }

    return ParseServerResult::Err(
        "Serverブロックが正常に終了しませんでした。\nserverブロックの中身が無い or } がありません");
}

// 設定ファイルを解析するメインの関数
ParseResult ParseConfig(const char *configPath)
{
    tokenizeResult tokensResult = tokenize(configPath);
    if (!tokensResult.ok())
        return ParseResult::Err(tokensResult.unwrapErr());
    std::vector<Tokenize> tokens = tokensResult.unwrap();

    std::vector<Server> servers;

    while (!tokens.empty())
    {
        Tokenize token = tokens.front();
        tokens.erase(tokens.begin());
        if (token.key == "server")
        {
            serverResult result = parseServer(token, tokens);
            if (!result.ok())
                return ParseResult::Err(result.unwrapErr());
            servers.push_back(result.unwrap());
        }
        else
        {
            return ParseResult::Err("Config: serverブロックがありません");
        }
    }
    return ParseResult::Ok(servers);
}
