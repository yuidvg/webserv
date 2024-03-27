#include "globals.hpp"

const HttpResponse getMethodNotAllowedResponse(const HttpRequest &httpRequest, const std::string &allowedMethods)
{
    return HttpResponse(httpRequest.sd, 405,
                        " <html> <head> <title>405 Method Not Allowed</title> </head> <body> <h1>405 Method Not "
                        "Allowed</h1> <p>The method is not allowed for the requested URL.<br /></p> </body> </html>",
                        "text/html", "", allowedMethods);
}

const int KQ = kqueue();

std::queue<const HttpRequest> HTTP_REQUESTS;


ConnectedInternetSockets CLIENT_SOCKETS;
ConnectedUnixSockets CGI_SOCKETS;

CgiHttpRequests CGI_HTTP_REQUESTS;

SocketBuffers SOCKET_BUFFERS;

Config CONFIG;
