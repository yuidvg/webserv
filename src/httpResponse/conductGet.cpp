#include ".hpp"
#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"

HttpResponse conductGet(const Uri &uri, const Location &location)
{
    const std::string targetResourcePath = resolvePath(uri.extraPath, location);
    const IsDirectoryResult isDirectoryResult = utils::isDirectory(targetResourcePath);
    if (isDirectoryResult.success)
    {
        const bool isDirectory = isDirectoryResult.value;
        if (isDirectory)
        {
            if (location.autoindex)
            {
                const DirectoryListHtmlResult directoryListHtmlResult = directoryListHtml(targetResourcePath);
                return directoryListHtmlResult.success
                           ? HttpResponse(SUCCESS, directoryListHtmlResult.value, "text/html")
                           : BAD_REQUEST_RESPONSE;
            }
            else
            {
                return BAD_REQUEST_RESPONSE;
            }
        }
        else // when targetResourcePath is assumed to be a file.
        {
            const FileContentResult fileContentResult = utils::fileContent(targetResourcePath);
            return fileContentResult.success
                       ? HttpResponse(SUCCESS, fileContentResult.value, utils::contentType(targetResourcePath))
                       : BAD_REQUEST_RESPONSE;
        }
    }
    else
    {
        return isDirectoryResult.error;
    }
}
