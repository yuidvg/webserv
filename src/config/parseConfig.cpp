#include "parseConfig.hpp"

// TODO: camelCaseに変更

template <typename T> Result<T, std::string> PullWord(std::istringstream &iss)
{
    T word;
    if (!(iss >> word))
    {
        return Result<T, std::string>::Error("Config: ディレクティブの引数が不正です");
    }
    std::string tmpStr;
    if (iss >> tmpStr)
    {
        std::ostringstream oss;
        oss << "Config: " << word << " ディレクティブの引数が多いです";
        return Result<T, std::string>::Error(oss.str());
    }
    return Result<T, std::string>::Success(word);
}

ErrorPageMapResult HandleErrorPageDirective(std::istringstream &iss, std::map<int, std::string> &errorPages)
{
    int errorCode; // ステータスコードを取得
    std::string errorPagePath;

    if (!(iss >> errorCode))
        return ErrorPageMapResult::Error("Config: error_code引数が不正です");
    if (!(iss >> errorPagePath))
        return ErrorPageMapResult::Error("Config: error_page_path");
    errorPages[errorCode] = errorPagePath;
    std::string tmpStr;
    if (iss >> tmpStr)
        return ErrorPageMapResult::Error("Config: error_pageの引数が多いです");
    return ErrorPageMapResult::Success("OK");
}

// TODO: 下記のようにコードを短くする。
// parsedLocation(std::ifstream &config_file)
// {
//     const std::vector<const std::string> lines = readLines(config_file);
//     const Result<std::string> nameResult = parseName(config_file, lines);
//     const Result<size_t> portResult = parsePort(config_file, lines);

//     if (!nameResult.success || !portResult.success
//         return ParseServerResult::Error(nameResult.error);
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
            Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.success)
                return ParseLocationResult::Error(result.error);
            root = result.value;
        }
        else if (key == "autoindex")
        {
            Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.success)
                return ParseLocationResult::Error(result.error);
            autoindex = (result.value == "on");
        }
        else if (key == "index")
        {
            Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.success)
                return ParseLocationResult::Error(result.error);
            index = result.value;
        }
        else if (key == "client_max_body_size")
        {
            Result<size_t, std::string> result = PullWord<size_t>(iss);
            if (!result.success)
                return ParseLocationResult::Error(result.error);
            clientMaxBodySize = result.value;
        }
        else if (key == "error_page")
        {

            ErrorPageMapResult errorPageMapResult = HandleErrorPageDirective(iss, errorPages);
            if (!errorPageMapResult.success)
                return ParseLocationResult::Error(errorPageMapResult.error);
        }
        else if (key == "allow_method")
        {
            // 許可されるHTTPメソッドをvectorに追加
            std::string method;
            int i = 0;
            while (iss >> method)
            {
                if (method != "GET" && method != "POST" && method != "DELETE")
                    return (ParseLocationResult::Error("Config: 許可されないHTTPメソッドが指定されています"));
                allowMethods.push_back(method);
                i++;
            }
            if (i > 3)
                return (ParseLocationResult::Error("Config: 許可されるHTTPメソッドが多すぎます"));
        }
        else if (key == "cgi_executor")
        {
            std::string cgiProgram;
            iss >> cgiProgram;
            cgiExtension = (cgiProgram);
        }
        else if (key == "upload_path")
        {
            Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.success)
                return ParseLocationResult::Error(result.error);
            uploadPath = result.value;
        }
        else if (key == "return")
        {
            int statusCode; // ステータスコードを取得
            if (!(iss >> statusCode))
                return ParseLocationResult::Error("Config: returnの引数が不正です");
            std::string redirectUrl;
            if (!(iss >> redirectUrl))
                return ParseLocationResult::Error("Config: returnの引数が不正です");
            std::string tmpStr;
            if (iss >> tmpStr)
                return ParseLocationResult::Error("Config: returnの引数が多いです");
            redirect[statusCode] = redirectUrl;
        }
        else if (key == "}")
        {
            return ParseLocationResult::Success(Location(path, root, autoindex, index, clientMaxBodySize, errorPages,
                                                         allowMethods, cgiExtension, uploadPath, redirect));
        }
        else if (key.empty() || key == "\t" || key == "\n")
        {
            continue;
        }
        else
        {
            return ParseLocationResult::Error("Location: 不正なディレクティブ");
        }
    }
    return ParseLocationResult::Error("Locationブロックで文字列が見つかりませんでした");
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
                    return ParseServerResult::Error("location.front_pathが指定されていません");
            }
            if (!(iss >> tmpStr) || tmpStr != "{")
                return ParseServerResult::Error("Config: locationブロックの開始が不正です");
            if (iss >> tmpStr)
                return ParseServerResult::Error("Config: locationの引数が多いです");
            ParseLocationResult location = ParseLocation(config_file, locationPath);
            if (!location.success)
                return ParseServerResult::Error(location.error);
            locations.push_back(location.value);
            if (config_file.eof())
                return ParseServerResult::Error("Config: サーバーブロックが終了していない");
        }
        else if (key == "server_name")
        {
            Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.success)
                return ParseServerResult::Error(result.error);
            name = result.value;
        }
        else if (key == "listen")
        {
            Result<int, std::string> result = PullWord<int>(iss);
            if (!result.success)
                return ParseServerResult::Error(result.error);
            port = result.value;
        }
        else if (key == "root")
        {
            Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.success)
                return ParseServerResult::Error(result.error);
            root = result.value;
        }
        else if (key == "error_page")
        {
            ErrorPageMapResult errorPageMapResult = HandleErrorPageDirective(iss, errorPages);
            if (!errorPageMapResult.success)
                return ParseServerResult::Error(errorPageMapResult.error);
        }
        else if (key == "client_max_body_size")
        {
            Result<size_t, std::string> result = PullWord<size_t>(iss);
            if (!result.success)
                return ParseServerResult::Error(result.error);
            clientMaxBodySize = result.value;
        }
        else if (key == "autoindex")
        {
            Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.success)
                return ParseServerResult::Error(result.error);
            std::string autoindex_value = result.value;
            autoindex = (autoindex_value == "on");
        }
        else if (key == "index")
        {
            Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.success)
                return ParseServerResult::Error(result.error);
            index = result.value;
        }
        else if (key == "}")
        {
            return ParseServerResult::Success(
                Server(name, port, root, errorPages, clientMaxBodySize, autoindex, index, locations));
        }
        else if (config_file.eof())
        {
            ParseServerResult::Error("Config: サーバーブロックが終了していない");
        }
        else if (key.empty() || key == "\t" || key == "\n")
        {
            continue;
        }
        else
        {
            ParseServerResult::Error("Server: 不正なディレクティブ");
        }
    }
    return ParseServerResult::Error("Serverブロックで文字列が見つかりませんでした");
}

// 設定ファイルを解析するメインの関数
ConfigResult parseConfig(const char *configPath)
{
    std::ifstream configFile(configPath);
    std::string line;
    std::vector<Server> servers;

    if (!configFile.is_open())
        return ConfigResult::Error("Failed to open file: " + std::string(configPath));
    while (std::getline(configFile, line))
    {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "server")
        {
            std::string tmpStr;
            if (!(iss >> tmpStr) || tmpStr != "{")
                return ConfigResult::Error("Config: serverブロックの開始が不正です");
            if (iss >> tmpStr)
                return ConfigResult::Error("Config: serverの引数が多いです");
            ParseServerResult serverResult = ParseServer(configFile);
            if (!serverResult.success)
                return ConfigResult::Error(serverResult.error);
            servers.push_back(serverResult.value);
        }
    }
    return ConfigResult::Success(servers);
}
