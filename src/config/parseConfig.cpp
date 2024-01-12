#include "parseConfig.hpp"

// parsedLocation(std::ifstream &config_file)
// {
//     const std::vector<const std::string> lines = readLines(config_file);
//     const Result<std::string> nameResult = parseName(config_file, lines);
//     const Result<size_t> portResult = parsePort(config_file, lines);

//     if (!nameResult.ok() || !portResult.ok()
//         return ParseServerResult::Err(nameResult.unwrapErr());
//     return {

//         ,
//     }
// }

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
        if (token.key == "root")
        {
            if (token.values.size() != 1)
                return ParseLocationResult::Err(token.key + "引数の数が不正です");
            root = token.values[0];
        }
        else if (token.key == "autoindex")
        {
            if (token.values.size() != 1)
                return ParseLocationResult::Err(token.key + "引数の数が不正です");
            autoindex = (token.values[0] == "on");
        }
        else if (token.key == "index")
        {
            if (token.values.size() != 1)
                return ParseLocationResult::Err(token.key + "引数の数が不正です");
            index = token.values[0];
        }
        else if (token.key == "client_max_body_size")
        {
            if (token.values.size() != 1)
                return ParseLocationResult::Err(token.key + "引数の数が不正です");

            clientMaxBodySize = std::atoi(token.values[0].c_str());
            if (clientMaxBodySize == 0)
                return ParseLocationResult::Err("client_max_body_sizeの値が不正です");
        }
        else if (token.key == "error_page")
        {
            if (token.values.size() != 2)
                return ParseLocationResult::Err("Config: error_pageの引数が多いです");
            std::string errorCodeStr = token.values[0];
            std::string errorPagePath = token.values[1];
            errorPages[std::stoi(errorCodeStr)] = errorPagePath;
        }
        else if (token.key == "allow_method")
        {
            if (token.values.size() > 3 || token.values.size() < 1)
                return ParseLocationResult::Err("Config: allow_methodの引数が不正です");
            // 許可されるHTTPメソッドをvectorに追加
            std::string method = token.values[0];
            if (method != "GET" && method != "POST" && method != "DELETE")
                return (ParseLocationResult::Err("Config: 許可されないHTTPメソッドが指定されています"));
            allowMethods.push_back(method);
        }
        else if (token.key == "cgi_executor")
        {
            if (token.values.size() != 1)
                return ParseLocationResult::Err("Config: cgi_executorの引数が多いです");
            cgiExtension = token.values[0];
        }
        else if (token.key == "upload_path")
        {
            if (token.values.size() != 1)
                return ParseLocationResult::Err(token.key + "引数の数が不正です");
            uploadPath = token.values[0];
        }
        else if (token.key == "return")
        {
            if (token.values.size() != 2)
                return ParseLocationResult::Err("Config: returnの引数が不正です");
            int statusCode = std::atoi(token.values[0].c_str());
            std::string redirectUrl = token.values[1];
            redirect[statusCode] = redirectUrl;
        }
        else if (token.key == "}")
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
        if (token.key == "location")
        {
            if (token.values.size() > 2)
                return ParseServerResult::Err("Config: locationの引数が多いです");
            if (tokens.front().key != OPEN_BRACKET && token.values[1] != OPEN_BRACKET)
                return ParseServerResult::Err("Config: locationブロックの開始が不正です");
            std::string locationsPath = token.values[0];
            if (tokens.front().key == OPEN_BRACKET)
            {
                token = tokens.front();
                tokens.erase(tokens.begin());
            }
            ParseLocationResult location = ParseLocation(tokens, locationsPath);
            if (!location.ok())
                return ParseServerResult::Err(location.unwrapErr());
            locations.push_back(location.unwrap());
        }
        else if (token.key == "server_name")
        {
            if (token.values.size() != 1)
                return ParseServerResult::Err("Config: server_nameの引数が多いです");
            name = token.values[0];
        }
        else if (token.key == "listen")
        {
            if (token.values.size() != 1)
                return ParseServerResult::Err("Config: listenの引数が多いです");
            std::string portStr = token.values[0];
            port = std::stoi(portStr);
        }
        else if (token.key == "error_page")
        {
            if (token.values.size() != 2)
                return ParseServerResult::Err("Config: error_pageの引数が多いです");
            std::string errorCodeStr = token.values[0];
            std::string errorPagePath = token.values[1];
            errorPages[std::stoi(errorCodeStr)] = errorPagePath;
        }
        else if (token.key == "client_max_body_size")
        {
            if (token.values.size() != 1)
                return ParseServerResult::Err(token.key + "引数の数が不正です");

            clientMaxBodySize = std::atoi(token.values[0].c_str());
            if (clientMaxBodySize == 0)
                return ParseServerResult::Err("client_max_body_sizeの値が不正です");
        }
        else if (token.key == "}")
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
            if (token.values.size() > 1)
                return ParseResult::Err("Config: serverの引数が多いです");
            if (tokens.front().key != OPEN_BRACKET && token.values[0] != OPEN_BRACKET)
                return ParseResult::Err("Config: serverブロックの開始が不正です");
            if (tokens.front().key == OPEN_BRACKET)
                tokens.erase(tokens.begin());
            ParseServerResult serverResult = ParseServer(tokens);
            if (!serverResult.ok())
                return ParseResult::Err(serverResult.unwrapErr());
            servers.push_back(serverResult.unwrap());
        }
        else
        {
            return ParseResult::Err("Config: serverブロックがありません");
        }
    }
    return ParseResult::Ok(servers);
}
