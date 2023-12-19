#include "parse_config.hpp"

void Config::PrintLocation(const Location &location)
{
    std::cout << "\x1b[33m";
    std::cout << "Location Path: " << location.path << std::endl;
    std::cout << "Root: " << location.root << std::endl;
    std::cout << "Autoindex: " << (location.autoindex ? "on" : "off") << std::endl;
    std::cout << "Index: ";
    for (size_t i = 0; i < location.index.size(); ++i)
    {
        std::cout << location.index[i] << (i < location.index.size() - 1 ? ", " : "");
    }
    std::cout << "\nMax Body Size: " << location.client_max_body_size << std::endl;

    // Error Pages
    std::cout << "Error Pages: ";
    for (std::map<int, std::string>::const_iterator it = location.error_page.begin();
         it != location.error_page.end(); ++it)
    {
        std::cout << it->first << " => " << it->second << ", ";
    }
    std::cout << std::endl;

    // Allow Methods
    std::cout << "Allowed Methods: ";
    for (size_t i = 0; i < location.allow_method.size(); ++i)
    {
        std::cout << location.allow_method[i] << (i < location.allow_method.size() - 1 ? ", " : "");
    }
    std::cout << std::endl;

    // CGI Path
    std::cout << "CGI Path: " << location.cgi_path << std::endl;

    // Upload Path
    std::cout << "Upload Path: " << location.upload_path << std::endl;

    // Redirect URL
    std::cout << "Redirect URL: " << location.redirect << std::endl;
    std::cout << "\x1b[0m";
}

void Config::PrintServer(const Server &server)
{
    std::cout << "\x1b[32m";
    std::cout << "Server Name: " << server.server_name << std::endl;
    std::cout << "Port: " << server.port << std::endl;

    // Error Pages
    std::cout << "Server Error Pages: ";
    for (std::map<int, std::string>::const_iterator it = server.error_page.begin();
         it != server.error_page.end(); ++it)
    {
        std::cout << it->first << " => " << it->second << ", ";
    }
    std::cout << std::endl;

    // Max Body Size
    std::cout << "Server Max Body Size: " << server.client_max_body_size << std::endl;

    // Autoindex
    std::cout << "Server Autoindex: " << (server.autoindex ? "on" : "off") << std::endl;

    // Default Index
    std::cout << "Server Default Index: ";
    for (size_t i = 0; i < server.index.size(); ++i)
    {
        std::cout << server.index[i] << (i < server.index.size() - 1 ? ", " : "");
    }
    std::cout << std::endl;

    // Locations
    for (size_t i = 0; i < server.locations.size(); ++i)
    {
        std::cout << "Location #" << (i + 1) << std::endl;
        PrintLocation(server.locations[i]);
    }
    std::cout << "\x1b[0m";
}

// Configクラスのコンストラクタ
Config::Config(const char *config_path)
{
    ParseConfig(config_path);
}

Config::~Config()
{
    std::cout << "delete config object" << std::endl;
}

void Config::InitializeServer(Server &server)
{
    server.server_name = "";
    server.port = 0;
    server.root = "";
    server.error_page.clear();
    server.client_max_body_size = 0;
    server.autoindex = false;
    server.index.clear();
    server.locations.clear();
}

void Config::InitializeLocation(Location &location)
{
    location.path = "";
    location.root = "";
    location.autoindex = false;
    location.index.clear();
    location.client_max_body_size = 0;
    location.error_page.clear();
    location.allow_method.clear();
    location.cgi_path = "";
    location.upload_path = "";
    location.redirect = "";
}

// 一般的なテンプレート関数
template <typename T>
T Config::PullWord(std::istringstream &iss, size_t num)
{
    // (void)num;
    std::vector<T> words;
    T word;
    /* c++標準ストリームの場合、この演算子>>はデフォルトで空白によって区切られたトークンを読み取ります。
    各単語や数値は空白によって分割され、それぞれがword変数に格納されます。 */
    while (iss >> word)
    {
        words.push_back(word);
    }

    if (words.size() >= num)
    {
        T num_word = words[0];
        // std::cout << "返すword: " << num_word << std::endl;
        return num_word;
    }
    else
    {
        std::cout << "指定したインデックスが大きすぎます" << std::endl;
        return T();
    }
}

// ロケーションブロックの設定を解析
void Config::ParseLocation(std::ifstream &config_file, Location &location)
{
    InitializeLocation(location);
    std::string line;
    while (std::getline(config_file, line))
    {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        // 各ディレクティブに対する処理を記述
        // 例: "root" ディレクティブ
        if (key == "root")
        {
            location.root = PullWord<std::string>(iss, 1);
            // その他の処理...
        }

        // locationブロックの終了
        if (key == "}")
        {
            break;
        }
    }
}

// サーバーブロックの設定を解析
void Config::ParseServer(std::ifstream &config_file, Server &server)
{
    InitializeServer(server);
    std::string line;
    while (std::getline(config_file, line))
    {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        // 各ディレクティブに対する処理を記述
        if (key == "server_name")
        {
            server.server_name = PullWord<std::string>(iss, 1);
        }
        if (key == "listen")
        {
            server.port = PullWord<int>(iss, 1);
            // その他の処理...
        }

        // locationブロックの処理
        if (key == "location")
        {
            Location location;
            location.path = PullWord<std::string>(iss, 1);
            ParseLocation(config_file, location);
            server.locations.push_back(location);
        }

        // serverブロックの終了
        if (key == "}")
        {
            break;
        }
    }
}

// 設定ファイルを解析するメイン関数
void Config::ParseConfig(const char *config_path)
{
    std::ifstream config_file(config_path);
    std::string line;
    if (!config_file.is_open())
    {
        std::cerr << "Failed to open file: " << config_path << std::endl;
        return;
    }

    while (std::getline(config_file, line))
    {
        std::istringstream iss(line);
        std::string key;
        iss >> key;

        if (key == "server")
        {
            Server server;
            ParseServer(config_file, server);
            webserver.servers.push_back(server);
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "config fileを指定してください" << std::endl;
        return 1;
    }

    const char *config_path = argv[1];
    Config config(config_path);
    std::cout << "server size = " << config.webserver.servers.size() << std::endl;
    // debug
    for (size_t i = 0; i < config.webserver.servers.size(); ++i)
    {
        std::cout << "Server #" << (i + 1) << std::endl;
        config.PrintServer(config.webserver.servers[i]);
    }

    return 0;
}
