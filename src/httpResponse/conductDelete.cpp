#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"
#include "../all.hpp"

HttpResponse conductDelete(const std::string &path, const ErrorPages &errorPages)
{
    const std::string relativePath = path.substr(1);
    if (remove(relativePath.c_str()) == 0)
        return (HttpResponse(SUCCESS, path, "text/html"));
    else
        return errorPages.at(BAD_REQUEST);
}
