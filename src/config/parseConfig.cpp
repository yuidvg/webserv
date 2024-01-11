#include "parseConfig.hpp"

//TODO: camelCaseに変更

template <typename T> utils::Result<T, std::string> PullWord(std::istringstream &iss)
{
    T word;
    if (!(iss >> word))
    {
        return utils::Result<T, std::string>::Err("Config: ディレクティブの引数が不正です");
    }
    std::string tmpStr;
    if (iss >> tmpStr)
    {
        std::ostringstream oss;
        oss << "Config: " << word << " ディレクティブの引数が多いです";
        return utils::Result<T, std::string>::Err(oss.str());
    }
    return utils::Result<T, std::string>::Ok(word);
}

ErrorPageMapResult HandleErrorPageDirective(std::istringstream &iss, std::map<int, std::string> &errorPages)
{
    int errorCode; // ステータスコードを取得
    std::string errorPagePath;

    if (!(iss >> errorCode))
        return ErrorPageMapResult::Err("Config: error_code引数が不正です");
    if (!(iss >> errorPagePath))
        return ErrorPageMapResult::Err("Config: error_page_path");
    errorPages[errorCode] = errorPagePath;
    std::string tmpStr;
    if (iss >> tmpStr)
        return ErrorPageMapResult::Err("Config: error_pageの引数が多いです");
    return ErrorPageMapResult::Ok("OK");
}

// TODO: 下記のようにコードを短くする。
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
ParseLocationResult ParseLocation(std::ifstream &config_file, std::string &locationPath)
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
    
    std::string line;

    while (std::getline(config_file, line))
    {
        std::istringstream tmp_iss(line);
        std::getline(tmp_iss, line, ';');
        std::istringstream iss(line);
        std::string key;
        iss >> key;
        if (key == "root")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseLocationResult::Err(result.unwrapErr());
            root = result.unwrap();
        }
        else if (key == "autoindex")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseLocationResult::Err(result.unwrapErr());
            autoindex = (result.unwrap() == "on");
        }
        else if (key == "index")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseLocationResult::Err(result.unwrapErr());
            index = result.unwrap();
        }
        else if (key == "client_max_body_size")
        {
            utils::Result<size_t, std::string> result = PullWord<size_t>(iss);
            if (!result.ok())
                return ParseLocationResult::Err(result.unwrapErr());
            clientMaxBodySize = result.unwrap();
        }
        else if (key == "error_page")
        {

            ErrorPageMapResult errorPageMapResult = HandleErrorPageDirective(iss, errorPages);
            if (!errorPageMapResult.ok())
                return ParseLocationResult::Err(errorPageMapResult.unwrapErr());
        }
        else if (key == "allow_method")
        {
            // 許可されるHTTPメソッドをvectorに追加
            std::string method;
            int i = 0;
            while (iss >> method)
            {
                if (method != "GET" && method != "POST" && method != "DELETE")
                    return (ParseLocationResult::Err("Config: 許可されないHTTPメソッドが指定されています"));
                allowMethods.push_back(method);
                i++;
            }
            if (i > 3)
                return (ParseLocationResult::Err("Config: 許可されるHTTPメソッドが多すぎます"));
        }
        else if (key == "cgi_executor")
        {
            std::string cgiProgram;
            iss >> cgiProgram;
            cgiExtension = (cgiProgram);
        }
        else if (key == "upload_path")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseLocationResult::Err(result.unwrapErr());
            uploadPath = result.unwrap();
        }
        else if (key == "return")
        {
            int statusCode; // ステータスコードを取得
                if (!(iss >> statusCode))
                return ParseLocationResult::Err("Config: returnの引数が不正です");
            std::string redirectUrl;
            if (!(iss >> redirectUrl))
                return ParseLocationResult::Err("Config: returnの引数が不正です");
            std::string tmpStr;
            if (iss >> tmpStr)
                return ParseLocationResult::Err("Config: returnの引数が多いです");
            redirect[statusCode] = redirectUrl;
}
        else if (key == "}")
        {
            return ParseLocationResult::Ok(Location(path, root, autoindex, index, clientMaxBodySize, errorPages, allowMethods, cgiExtension, uploadPath, redirect));
        }
        else if (key.empty() || key == "\t" || key == "\n")
        {
            continue;
        }
        else
        {
            return ParseLocationResult::Err("Location: 不正なディレクティブ");
        }
    }
    return ParseLocationResult::Err("Locationブロックで文字列が見つかりませんでした");
}

// サーバーブロックの設定を解析
ParseServerResult ParseServer(std::ifstream &config_file)
{
    std::string line;
    std::string name = "";
    size_t port = 80;
    std::string root = "";
    std::map<int, std::string> errorPages;
    size_t clientMaxBodySize = 1048576;
    bool autoindex = false;
    std::string index = "index.html";
    std::vector<Location> locations;

    while (std::getline(config_file, line))
    {
        std::istringstream tmp_iss(line);
        std::getline(tmp_iss, line, ';');
        std::istringstream iss(line);
        std::string key;
        iss >> key;
        if (key == "location")
        {

            std::string locationPath;
            std::string tmpStr;
            if (key == "location")
            {
                if (!(iss >> locationPath))
                    return ParseServerResult::Err("location.front_pathが指定されていません");
            }
            if (!(iss >> tmpStr) || tmpStr != "{")
                return ParseServerResult::Err("Config: locationブロックの開始が不正です");
            if (iss >> tmpStr)
                return ParseServerResult::Err("Config: locationの引数が多いです");
            ParseLocationResult location = ParseLocation(config_file, locationPath);
            if(!location.ok())
                return ParseServerResult::Err(location.unwrapErr());
            locations.push_back(location.unwrap());
            if (config_file.eof())
                return ParseServerResult::Err("Config: サーバーブロックが終了していない");
        }
        else if (key == "server_name")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            name = result.unwrap();
        }
        else if (key == "listen")
        {
            utils::Result<int, std::string> result = PullWord<int>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            port = result.unwrap();
        }
        else if (key == "root")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            root = result.unwrap();
        }
        else if (key == "error_page")
        {
            ErrorPageMapResult errorPageMapResult = HandleErrorPageDirective(iss, errorPages);
            if (!errorPageMapResult.ok())
                return ParseServerResult::Err(errorPageMapResult.unwrapErr());
        }
        else if (key == "client_max_body_size")
        {
            utils::Result<size_t, std::string> result = PullWord<size_t>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            clientMaxBodySize = result.unwrap();
        }
        else if (key == "autoindex")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            std::string autoindex_value = result.unwrap();
            autoindex = (autoindex_value == "on");
        }
        else if (key == "index")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            index = result.unwrap();
        }
        else if (key == "}")
        {
            return ParseServerResult::Ok(Server(name, port, root, errorPages, clientMaxBodySize, autoindex, index, locations));
        }
        else if (config_file.eof())
        {
            ParseServerResult::Err("Config: サーバーブロックが終了していない");
        }
        else if (key.empty() || key == "\t" || key == "\n")
        {
            continue;
        }
        else
        {
            ParseServerResult::Err("Server: 不正なディレクティブ");
        }
    }
    return ParseServerResult::Err("Serverブロックで文字列が見つかりませんでした");
}

// 設定ファイルを解析するメインの関数
ParseResult ParseConfig(const char *configPath)
{
    std::ifstream configFile(configPath);
    std::string line;
    std::vector<Server> servers;

    if (!configFile.is_open())
        return ParseResult::Err("Failed to open file: " + std::string(configPath));
    while (std::getline(configFile, line))
    {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "server")
        {
            std::string tmpStr;
            if (!(iss >> tmpStr) || tmpStr != "{")
                return ParseResult::Err("Config: serverブロックの開始が不正です");
            if (iss >> tmpStr)
                return ParseResult::Err("Config: serverの引数が多いです");
            ParseServerResult serverResult = ParseServer(configFile);
            if (!serverResult.ok())
                return ParseResult::Err(serverResult.unwrapErr());
            servers.push_back(serverResult.unwrap());
        }
    }
    return ParseResult::Ok(servers);
}
