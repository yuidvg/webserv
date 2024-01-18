#ifndef SERVER_HPP
#define SERVER_HPP

#include "Location.hpp"
#include "external.hpp"

struct Server
{
    const std::string name; // サーバー名
    const size_t port;
    const std::map<int, std::string> errorPages;
    const size_t clientMaxBodySize; // リクエストボディ最大サイズ
    const std::vector<Location> locations;
    Server();
    Server(const std::string name, const size_t port, const std::map<int, std::string> errorPages,
           const size_t clientMaxBodySize, const std::vector<Location> locations);
};

typedef std::vector<const Server> Servers;

#endif