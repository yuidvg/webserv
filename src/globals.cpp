#include "globals.hpp"

const HttpResponse METHOD_NOT_ALLOWED_RESPONSE(const std::string &allowedMethods)
{
    return HttpResponse(406,
                        " <html> <head> <title>405 Method Not Allowed</title> </head> <body> <h1>405 Method Not "
                        "Allowed</h1> <p>The method is not allowed for the requested URL.<br /></p> </body> </html>",
                        "text/html", "", allowedMethods);
}
