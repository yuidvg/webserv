#include ".hpp"

HttpResponse redirectResponse(const std::string& redirectUrl)
{
    Headers headers("", "0", redirectUrl);
    return HttpResponse(REDIRECTION,headers, "");
}