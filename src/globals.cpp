#include "globals.hpp"

const HttpResponse METHOD_NOT_ALLOWED_RESPONSE(const std::string &allowedMethods)
{
    return HttpResponse(405,
                        " <html> <head> <title>405 Method Not Allowed</title> </head> <body> <h1>405 Method Not "
                        "Allowed</h1> <p>The method is not allowed for the requested URL.<br /></p> </body> </html>",
                        "text/html", "", allowedMethods);
}

const int KQ = kqueue();

HttpRequests HTTP_REQUESTS;

ConnectedInternetSockets CLIENT_SOCKETS;
ConnectedInternetSockets CGI_SOCKETS;

SocketBuffers SOCKET_BUFFERS;

Config CONFIG;
