
#include "print_config.hpp"

void PrintLocation(const Location& location)
{
    std::cout << "\x1b[33m";
    std::cout << "Location Front_Path: " << location.front_path << std::endl;
    std::cout << "Location Back_Path: " << location.back_path << std::endl;
    std::cout << "Root: " << location.root << std::endl;
    std::cout << "Autoindex: " << (location.autoindex ? "on" : "off") << std::endl;
    std::cout << "Index: " << location.index << std::endl;
    std::cout << "Max Body Size: " << location.client_max_body_size << std::endl;

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
    std::cout << "CGI Executor: ";
    for (size_t i = 0; i < location.cgi_executor.size(); ++i)
    {
        std::cout << location.cgi_executor[i] << (i < location.cgi_executor.size() - 1 ? ", " : "");
    }
    std::cout << std::endl;

    // Upload Path
    std::cout << "Upload Path: " << location.upload_path << std::endl;

    // Redirect URL
    std::cout << "Redirect URL: ";
    for (std::map<int, std::string>::const_iterator it = location.redirect.begin();
        it != location.redirect.end(); ++it)
    {
        std::cout << it->first << " => " << it->second << ", ";
    }
    std::cout << std::endl;
    std::cout << NORMAL;
}

void PrintServer(const Server& server)
{
    std::cout << "\x1b[32m";
    std::cout << "Server Name: " << server.name << std::endl;
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

    std::cout << "Index: " << server.index << std::endl;

    // Locations
    for (size_t i = 0; i < server.locations.size(); ++i)
    {
        std::cout << "\x1b[0m";
        std::cout << "Location #" << (i + 1) << std::endl;
        PrintLocation(server.locations[i]);
    }
    std::cout << "\x1b[0m";
}

void DebugPrint(std::vector<Server> servers)
{
    std::cout << "server size = " << servers.size() << std::endl;
    // debug
    for (size_t i = 0; i < servers.size(); ++i)
    {
        std::cout << "Server #" << (i + 1) << std::endl;
        PrintServer(servers[i]);
    }
}