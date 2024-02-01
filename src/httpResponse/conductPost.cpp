#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

HttpResponse conductPost(const HttpRequest &request, const Uri &uri, const Location &location)
{
    const std::string targetResourcePath = resolvePath(uri.extraPath, location);
    const IsDirectoryResult isDirectoryResult = utils::isDirectory(targetResourcePath);
    if (isDirectoryResult.success)
    {
        const bool isDirectory = isDirectoryResult.value;
        if (isDirectory)
        {
            return BAD_REQUEST_RESPONSE;
        }
        else // when targetResourcePath is assumed to be a file.
        {
            return utils::writeToFile(targetResourcePath, request.body);
        }
    }
    else
    {
        return isDirectoryResult.error;
    }
}
