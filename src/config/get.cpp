#include "../all.hpp"

Server getServer(const HttpRequest &httpRequest)
{
    return (CONFIG.getServer(httpRequest.host, httpRequest.socket.serverPort));
}

Location getLocation(const HttpRequest &httpRequest)
{
    return getServer(httpRequest).getLocation(httpRequest.target);
}

