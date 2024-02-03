#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

HttpResponse conductDelete(const Uri &uri)
{
    if (remove(uri.extraPath.c_str()) == 0)
        return SUCCESS_RESPONSE;
    else
        return BAD_REQUEST_RESPONSE;
}
