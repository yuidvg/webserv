#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

HttpResponse conductDelete(const Uri &uri, const Location &location)
{
    const std::string targetResourcePath = resolvePath(uri.extraPath, location);
    if (remove(targetResourcePath.c_str()) == 0)
        return SUCCESS_RESPONSE;
    else
        return BAD_REQUEST_RESPONSE;
}
