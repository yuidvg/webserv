#include "all.hpp"

namespace utils
{

Server matchedServer(const std::string host, const Servers servers, const Socket socket)
{
    Server server;
    for (Servers::const_iterator it = servers.begin(); it != servers.end(); it++)
    {
        if (it->name == host && it->port == socket.port)
        {
            server = *it;
            break;
        }
    }
    return server;
}

} // namespace utils
