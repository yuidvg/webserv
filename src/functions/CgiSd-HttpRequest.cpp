#include "../all.hpp"

HttpRequest getHttpRequest(const int cgiSd)
{
    CgiHttpRequests::const_iterator it = CGI_HTTP_REQUESTS.find(cgiSd);
    if (it != CGI_HTTP_REQUESTS.end())
    {
        return it->second;
    }
    else
    {
        std::cerr << "Failed to get HTTP request" << std::endl;
        return HttpRequest();
    }
}
