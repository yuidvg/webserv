#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

HttpResponse conductDelete(const std::string &path)
{
    const std::string relativePath = path.substr(1);
    if (remove(relativePath.c_str()) == 0)
        return (HttpResponse(SUCCESS, path, "text/html"));
    else
        return BAD_REQUEST_RESPONSE;
}
