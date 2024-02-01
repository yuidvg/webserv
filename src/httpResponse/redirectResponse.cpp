#include ".hpp"

HttpResponse redirectResponse(const std::string &redirectUrl)
{
    return HttpResponse(REDIRECTION, "", "", redirectUrl);
}
