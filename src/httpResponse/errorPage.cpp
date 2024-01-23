#include "build.hpp"

static HttpResponse createDefaultHttpErrorPage(const int statusCode)
{
    if (statusCode <= 400 && statusCode < 500)
        return BAD_REQUEST_RESPONSE;
    else if (statusCode <= 500 && statusCode < 600)
        return SERVER_ERROR_RESPONSE;
}

HttpResponse createHttpErrorPage(const int statusCode, const HttpRequest &request, const Server &server)
{
    std::map<int, std::string>::const_iterator it = server.errorPages.find(statusCode);

    if (it != server.errorPages.end())
    {
        std::ifstream file(it->second);
        if (file)
        {
            // fileの中身を読み込む
        }
        else
        {
            return createDefaultHttpErrorPage(statusCode);
        }
    }
    else
    {
        return createDefaultHttpErrorPage(statusCode);
    }
}
