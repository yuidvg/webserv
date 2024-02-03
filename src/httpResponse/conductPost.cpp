#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

HttpResponse conductPost(const HttpRequest &request, const Uri &uri)
{
    const IsDirectoryResult isDirectoryResult = utils::isDirectory(uri.extraPath);
    if (isDirectoryResult.success)
    {
        const bool isDirectory = isDirectoryResult.value;
        if (isDirectory)
        {
            return BAD_REQUEST_RESPONSE;
        }
        else // when uri.extraPath is assumed to be a file.
        {
            return utils::writeToFile(uri.extraPath, request.body);
        }
    }
    else
    {
        return isDirectoryResult.error;
    }
}
