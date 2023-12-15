#include "parse_config.hpp"

// configファイルを解析してWebServerオブジェクトを構築する関数
WebServer ParseConfig(const std::string &filename)
{
    WebServer web_server;
    std::ifstream config_file(filename.c_str());
    std::string line;

    if (!config_file.is_open())
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return web_server; // 空のWebServerを返す
    }

    Server current_server;
    Location current_location;
    bool in_location = false;

    while (std::getline(config_file, line))
    {
        std::istringstream iss(line);
        std::string key;
        iss >> key;
        // printf("key: %s\n", key.c_str());
        printf("iss: %s\n", iss.str().c_str());
        if (key == "server")
        {
            if (in_location)
            {
                current_server.locations.push_back(current_location);
                current_location = Location();
                in_location = false;
            }
            web_server.servers.push_back(current_server);
            current_server = Server();
        }
        else if (key == "listen")
        {
            iss >> current_server.port;
        }
        else if (key == "location")
        {
            if (in_location)
            {
                current_server.locations.push_back(current_location);
            }
            iss >> current_location.path;
            in_location = true;
            current_location = Location();
        }
        else if (key == "allow_method")
        {
            std::string method;
            iss >> method;
            current_location.allow_method.push_back(method);
        }
        else if (key == "root")
        {
            iss >> current_location.root;
        }
        else if (key == "index")
        {
            std::string index_file;
            iss >> index_file;
            current_location.index.push_back(index_file);
        }
        else if (key == "server_name")
        {
            iss >> current_server.server_name;
        }
        else if (key == "autoindex")
        {
            std::string value;
            iss >> value;
            current_location.autoindex = (value == "on");
        }
        else if (key == "error_page")
        {
            int error_code;
            std::string error_page_path;
            iss >> error_code >> error_page_path;
            if (in_location)
            {
                current_location.error_page[error_code] = error_page_path;
            }
            else
            {
                current_server.error_page[error_code] = error_page_path;
            }
        }
        else if (key == "client_max_body_size")
        {
            size_t size;
            iss >> size;
            if (in_location)
            {
                current_location.client_max_body_size = size;
            }
            else
            {
                current_server.client_max_body_size = size;
            }
        }
        else if (key == "cgi_path")
        {
            iss >> current_location.cgi_path;
        }
        else if (key == "upload_path")
        {
            iss >> current_location.upload_path;
        }
        else if (key == "redirect")
        {
            iss >> current_location.redirect;
        }
    }

    if (in_location)
    {
        current_server.locations.push_back(current_location);
    }

    return web_server;
}

void printLocation(const Location &location)
{
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
}

void printServer(const Server &server)
{
    std::cout << "Server Name: " << server.server_name << std::endl;
    std::cout << "Host: " << server.host << std::endl;
    std::cout << "IP Address: " << server.ip_addr << std::endl;
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
        printLocation(server.locations[i]);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "config filewを指定してください" << std::endl;
        return 1;
    }

    std::string config_file = argv[1];
    WebServer web_server = ParseConfig(config_file);

    // debug
    for (size_t i = 0; i < web_server.servers.size(); ++i)
    {
        std::cout << "Server #" << (i + 1) << std::endl;
        printServer(web_server.servers[i]);
    }

    return 0;
}
