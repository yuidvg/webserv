#include "parseConfig.hpp"

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

// ロケーションブロックの設定を解析
ParseRoutesResult ParseLocation(std::ifstream &config_file, Location &location)
{
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
                return ParseRoutesResult::Err(result.unwrapErr());
            location.root = result.unwrap();
        }
        else if (key == "autoindex")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseRoutesResult::Err(result.unwrapErr());
            location.autoindex = (result.unwrap() == "on");
        }
        else if (key == "index")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseRoutesResult::Err(result.unwrapErr());
            location.index = result.unwrap();
        }
        else if (key == "clientMaxBodySize")
        {
            utils::Result<size_t, std::string> result = PullWord<size_t>(iss);
            if (!result.ok())
                return ParseRoutesResult::Err(result.unwrapErr());
            location.clientMaxBodySize = result.unwrap();
        }
        else if (key == "errorPages")
        {

            ErrorPageMapResult errorPageMapResult = HandleErrorPageDirective(iss, location.errorPages);
            if (!errorPageMapResult.ok())
                return ParseRoutesResult::Err(errorPageMapResult.unwrapErr());
        }
        else if (key == "allowMethod")
        {
            // 許可されるHTTPメソッドをvectorに追加
            std::string method;
            int i = 0;
            while (iss >> method)
            {
                if (method != "GET" && method != "POST" && method != "DELETE")
                    return (ParseRoutesResult::Err("Config: 許可されないHTTPメソッドが指定されています"));
                location.allowMethods.push_back(method);
                i++;
            }
            if (i > 3)
                return (ParseRoutesResult::Err("Config: 許可されるHTTPメソッドが多すぎます"));
        }
        else if (key == "cgiExtension")
        {
            std::string cgiProgram;
            iss >> cgiProgram;
            location.cgiExtension = (cgiProgram);
        }
        else if (key == "upload_path")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseRoutesResult::Err(result.unwrapErr());
            location.uploadPath = result.unwrap();
        }
        else if (key == "return")
        {
            int statusCode; // ステータスコードを取得
                if (!(iss >> statusCode))
                return ParseRoutesResult::Err("Config: returnの引数が不正です");
            std::string redirectUrl;
            if (!(iss >> redirectUrl))
                return ParseRoutesResult::Err("Config: returnの引数が不正です");
            std::string tmpStr;
            if (iss >> tmpStr)
                return ParseRoutesResult::Err("Config: returnの引数が多いです");
            location.redirect[statusCode] = redirectUrl;
}
        else if (key == "}")
        {
            break;
        }
        else if (key.empty() || key == "\t" || key == "\n")
        {
            continue;
        }
        else
        {
            return ParseRoutesResult::Err("不正なディレクティブ");
        }
    }
    return ParseRoutesResult::Ok("OK");
}

// サーバーブロックの設定を解析
ParseServerResult ParseServer(std::ifstream &config_file)
{
    std::string line;
    Server server;
    while (std::getline(config_file, line))
    {
        std::istringstream tmp_iss(line);
        std::getline(tmp_iss, line, ';');
        std::istringstream iss(line);
        std::string key;
        iss >> key;
        // 各ディレクティブに対する処理を記述
        if (key == "location" || key == "location_back")
        {

            Location location;
            std::string tmpStr;
            if (key == "location")
            {
                if (!(iss >> location.path))
                    return ParseServerResult::Err("location.front_pathが指定されていません");
            }
            if (!(iss >> tmpStr) || tmpStr != "{")
                return ParseServerResult::Err("Config: locationブロックの開始が不正です");
            if (iss >> tmpStr)
                return ParseServerResult::Err("Config: locationの引数が多いです");
            ParseLocation(config_file, location);
            server.locations.push_back(location);
            if (config_file.eof())
                return ParseServerResult::Err("Config: サーバーブロックが終了していない");
        }
        else if (key == "server_name")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            server.name = result.unwrap();
        }
        else if (key == "listen")
        {
            utils::Result<int, std::string> result = PullWord<int>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            server.port = result.unwrap();
        }
        else if (key == "root")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            server.root = result.unwrap();
        }
        else if (key == "errorPages")
        {
            ErrorPageMapResult errorPageMapResult = HandleErrorPageDirective(iss, server.errorPages);
            if (!errorPageMapResult.ok())
                return ParseServerResult::Err(errorPageMapResult.unwrapErr());
        }
        else if (key == "clientMaxBodySize")
        {
            utils::Result<size_t, std::string> result = PullWord<size_t>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            server.clientMaxBodySize = result.unwrap();
        }
        else if (key == "autoindex")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            std::string autoindex_value = result.unwrap();
            server.autoindex = (autoindex_value == "on");
        }
        else if (key == "index")
        {
            utils::Result<std::string, std::string> result = PullWord<std::string>(iss);
            if (!result.ok())
                return ParseServerResult::Err(result.unwrapErr());
            server.index = result.unwrap();
        }
        else if (key == "}")
        {
            return (ParseServerResult::Ok(server));
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
            ParseServerResult::Err("Config: 不正なディレクティブ");
        }
    }
    return ParseServerResult::Ok(server);
}

// 設定ファイルを解析するメインの関数
ConfigResult ParseConfig(const char *configPath)
{
    std::ifstream configFile(configPath);
    std::string line;
    std::vector<Server> servers;

    if (!configFile.is_open())
        return ConfigResult::Err("Failed to open file: " + std::string(configPath));
    while (std::getline(configFile, line))
    {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "server")
        {
            std::string tmpStr;
            if (!(iss >> tmpStr) || tmpStr != "{")
                return ConfigResult::Err("Config: serverブロックの開始が不正です");
            if (iss >> tmpStr)
                return ConfigResult::Err("Config: serverの引数が多いです");
            ParseServerResult serverResult = ParseServer(configFile);
            if (!serverResult.ok())
                return ConfigResult::Err(serverResult.unwrapErr());
            servers.push_back(serverResult.unwrap());
        }
    }
    return ConfigResult::Ok(servers);
}
