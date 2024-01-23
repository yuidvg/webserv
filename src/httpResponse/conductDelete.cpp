#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

HttpResponse conductDelete(const HttpRequest &request, const Location &location)
{
    const std::string targetResourcePath = comply(request.target, location);
    if (remove(targetResourcePath.c_str()) == 0)
        return SUCCESS_RESPONSE;
    else
        return BAD_REQUEST_RESPONSE;
}
