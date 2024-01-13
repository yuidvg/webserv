#include "parseConfig.hpp"
#include "parseDirective.hpp"

using namespace parseDirective;

StringResult checkBracketsBalance(const std::vector<Token> &tokens)
{
    int brackets = 0;
    for (std::vector<Token>::const_iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        if (it->values[0] == OPEN_BRACKET)
            brackets++;
        else if (it->values[0] == CLOSE_BRACKET)
            brackets--;
        else
        {
            for (std::vector<std::string>::const_iterator valueIt = it->values.begin(); valueIt != it->values.end();
                 ++valueIt)
            {
                if (*valueIt == OPEN_BRACKET)
                    brackets++;
                else if (*valueIt == CLOSE_BRACKET)
                    brackets--;
            }
        }
        // サーバーコンテキスト内での2回ネストされたコンテキストは許可されません
        if (brackets > 2)
            return StringResult::Err("Too many nested contexts in config file");
    }
    if (brackets != 0)
        return StringResult::Err("Invalid number of brackets " + utils::to_string(brackets) + " in config file");
    return StringResult::Ok("ok");
}

void removeSemicolonFromToken(Token &token)
{
    if (token.values[0] == SERVER_NAME || token.values[0] == LISTEN || token.values[0] == ALLOW_METHOD ||
        token.values[0] == CGI_EXECUTOR || token.values[0] == UPLOAD_PATH || token.values[0] == RETURN ||
        token.values[0] == ROOT || token.values[0] == ERROR_PAGE || token.values[0] == CLIENT_MAX_BODY_SIZE ||
        token.values[0] == INDEX || token.values[0] == AUTOINDEX)
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
    }
}

// ロケーションブロックの設定を解析
ParseLocationResult parseLocationContext(std::vector<Token> &tokens, std::string &locationPath)
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
        Token token = tokens.front();
        tokens.erase(tokens.begin());
        if (token.values[0] == ROOT)
        {
            RootResult rootRes = parseRootDirective(token);
            if (!rootRes.ok())
                return ParseLocationResult::Err(rootRes.unwrapErr());
            root = rootRes.unwrap();
        }
        else if (token.values[0] == AUTOINDEX)
        {
            AutoindexResult autoindexRes = parseAutoindexDirective(token);
            if (!autoindexRes.ok())
                return ParseLocationResult::Err(autoindexRes.unwrapErr());
            autoindex = autoindexRes.unwrap();
        }
        else if (token.values[0] == INDEX)
        {
            IndexResult indexRes = parseIndexDirective(token);
            if (!indexRes.ok())
                return ParseLocationResult::Err(indexRes.unwrapErr());
            index = indexRes.unwrap();
        }
        else if (token.values[0] == CLIENT_MAX_BODY_SIZE)
        {
            ClientMaxBodySizeResult clientMaxBodySizeRes = parseClientMaxBodySize(token);
            if (!clientMaxBodySizeRes.ok())
                return ParseLocationResult::Err(clientMaxBodySizeRes.unwrapErr());
            clientMaxBodySize = clientMaxBodySizeRes.unwrap();
        }
        else if (token.values[0] == ERROR_PAGE)
        {
            ErrorPagesResult errorPageRes = parseErrorPage(token);
            if (!errorPageRes.ok())
                return ParseLocationResult::Err(errorPageRes.unwrapErr());
            std::map<int, std::string> newErrorPages = errorPageRes.unwrap();
            errorPages.insert(newErrorPages.begin(), newErrorPages.end());
        }
        else if (token.values[0] == ALLOW_METHOD)
        {
            AllowMethodsResult allowMethodsRes = parseAllowMethodDirective(token);
            if (!allowMethodsRes.ok())
                return ParseLocationResult::Err(allowMethodsRes.unwrapErr());
            allowMethods = allowMethodsRes.unwrap();
        }
        else if (token.values[0] == CGI_EXECUTOR)
        {
            CgiExtensionResult cgiExtensionRes = parseCgiExecutorDirective(token);
            if (!cgiExtensionRes.ok())
                return ParseLocationResult::Err(cgiExtensionRes.unwrapErr());
            cgiExtension = cgiExtensionRes.unwrap();
        }
        else if (token.values[0] == UPLOAD_PATH)
        {
            UploadPathResult uploadPathRes = parseUploadPathDirective(token);
            if (!uploadPathRes.ok())
                return ParseLocationResult::Err(uploadPathRes.unwrapErr());
            uploadPath = uploadPathRes.unwrap();
        }
        else if (token.values[0] == RETURN)
        {
            RedirectResult redirectRes = parseReturnDirective(token);
            if (!redirectRes.ok())
            {
                return ParseLocationResult::Err(redirectRes.unwrapErr());
            }
            std::map<int, std::string> newRedirect = redirectRes.unwrap();
            redirect.insert(newRedirect.begin(), newRedirect.end());
        }

        else if (token.values[0] == CLOSE_BRACKET)
        {
            return ParseLocationResult::Ok(Location(path, root, autoindex, index, clientMaxBodySize, errorPages,
                                                    allowMethods, cgiExtension, uploadPath, redirect));
        }
    }
    return ParseLocationResult::Err("Locationブロックで文字列が見つかりませんでした");
}

// サーバーブロックの設定を解析
ParseServerResult parseServerContext(std::vector<Token> &tokens)
{
    std::string name = "";
    size_t port = 80;
    std::string root = "";
    std::map<int, std::string> errorPages;
    size_t clientMaxBodySize = 1048576;
    bool autoindex = false;
    std::string index = "index.html";
    std::vector<Location> locations;

    while (!tokens.empty())
    {
        Token token = tokens.front();
        tokens.erase(tokens.begin());
        if (token.values[0] == LOCATION)
        {
            LocationResult locationRes = parseLocationDirective(token, tokens);
            if (!locationRes.ok())
                return ParseServerResult::Err(locationRes.unwrapErr());
            locations.push_back(locationRes.unwrap());
        }
        else if (token.values[0] == SERVER_NAME)
        {
            NameResult nameRes = parseServerName(token);
            if (!nameRes.ok())
                return ParseServerResult::Err(nameRes.unwrapErr());
            name = nameRes.unwrap();
        }
        else if (token.values[0] == LISTEN)
        {
            PortResult listenRes = parseListen(token);
            if (!listenRes.ok())
                return ParseServerResult::Err(listenRes.unwrapErr());
            port = listenRes.unwrap();
        }
        if (token.values[0] == ROOT)
        {
            RootResult rootRes = parseRootDirective(token);
            if (!rootRes.ok())
                return ParseServerResult::Err(rootRes.unwrapErr());
            root = rootRes.unwrap();
        }
        else if (token.values[0] == AUTOINDEX)
        {
            AutoindexResult autoindexRes = parseAutoindexDirective(token);
            if (!autoindexRes.ok())
                return ParseServerResult::Err(autoindexRes.unwrapErr());
            autoindex = autoindexRes.unwrap();
        }
        else if (token.values[0] == INDEX)
        {
            IndexResult indexRes = parseIndexDirective(token);
            if (!indexRes.ok())
                return ParseServerResult::Err(indexRes.unwrapErr());
            index = indexRes.unwrap();
        }
        else if (token.values[0] == ERROR_PAGE)
        {
            ErrorPagesResult errorPageRes = parseErrorPage(token);
            if (!errorPageRes.ok())
                return ParseServerResult::Err(errorPageRes.unwrapErr());
            std::map<int, std::string> newErrorPages = errorPageRes.unwrap();
            errorPages.insert(newErrorPages.begin(), newErrorPages.end());
        }
        else if (token.values[0] == CLIENT_MAX_BODY_SIZE)
        {
            ClientMaxBodySizeResult clientMaxBodySizeRes = parseClientMaxBodySize(token);
            if (!clientMaxBodySizeRes.ok())
                return ParseServerResult::Err(clientMaxBodySizeRes.unwrapErr());
            clientMaxBodySize = clientMaxBodySizeRes.unwrap();
        }
        else if (token.values[0] == CLOSE_BRACKET)
        {
            return ParseServerResult::Ok(
                Server(name, port, root, errorPages, clientMaxBodySize, autoindex, index, locations));
        }
    }

    return ParseServerResult::Err(
        "Serverブロックが正常に終了しませんでした。\nserverブロックの中身が無い or } がありません");
}

// 設定ファイルを解析するメインの関数
ParseResult parseConfig(const char *configPath)
{
    TokenResult tokensResult = tokenize(configPath);
    if (!tokensResult.ok())
        return ParseResult::Err(tokensResult.unwrapErr());
    std::vector<Token> tokens = tokensResult.unwrap();
    for (std::vector<Token>::iterator it = tokens.begin(); it != tokens.end(); ++it)
    {
        // トークンの末尾にあるセミコロンを削除
        if (!it->values.empty())
            removeSemicolonFromToken(*it);
    }
    // ブロックの数が合っているか確認
    StringResult result = checkBracketsBalance(tokens);
    if (!result.ok())
        return ParseResult::Err(result.unwrapErr());

    std::vector<Server> servers;

    while (!tokens.empty())
    {
        Token token = tokens.front();
        tokens.erase(tokens.begin());
        if (token.values[0] == "server")
        {
            ServerResult serverRes = parseServer(token, tokens);
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
