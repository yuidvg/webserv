#include "globals.hpp"
#include "all.hpp"

const HttpResponse getMethodNotAllowedResponse(const HttpRequest &httpRequest, const std::string &allowedMethods)
{
    return HttpResponse(httpRequest.socket.descriptor, 405,
                        " <html> <head> <title>405 Method Not Allowed</title> </head> <body> <h1>405 Method Not "
                        "Allowed</h1> <p>The method is not allowed for the requested URL.<br /></p> </body> </html>",
                        "text/html", "", allowedMethods);
}

const int KQ = kqueue();

Config CONFIG;
Sockets SOCKETS;

CgiHttpRequests CGI_HTTP_REQUESTS;
