#include ".hpp"
#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"

HttpResponse conductGet(const Uri &uri, const Location &location, const Server &server)
{
    const IsDirectoryResult isDirectoryResult = utils::isDirectory(uri.extraPath);
    if (isDirectoryResult.success)
    {
        const bool isDirectory = isDirectoryResult.value;
        if (isDirectory)
        {
            if (location.autoindex)
            {
                const DirectoryListHtmlResult directoryListHtmlResult = directoryListHtml(uri.extraPath);
                return directoryListHtmlResult.success
                           ? HttpResponse(SUCCESS, directoryListHtmlResult.value, "text/html")
                           : utils::generateErrorResponse(BAD_REQUEST, server);
            }
            else if (location.index.size() > 0)
            {
                const std::string indexPath = uri.extraPath + location.index;
                const FileContentResult fileContentResult = utils::fileContent(indexPath);
                return fileContentResult.success
                           ? HttpResponse(SUCCESS, fileContentResult.value, utils::contentType(indexPath))
                           : BAD_REQUEST_RESPONSE;
            }
            else
            {
                return utils::generateErrorResponse(BAD_REQUEST, server);
            }
        }
        else // when uri.extraPath is assumed to be a file.
        {
            const FileContentResult fileContentResult = utils::fileContent(uri.extraPath);
            return fileContentResult.success
                       ? HttpResponse(SUCCESS, fileContentResult.value, utils::contentType(uri.extraPath))
                       : utils::generateErrorResponse(BAD_REQUEST, server);
        }
    }
    else
    {
        return utils::generateErrorResponse(isDirectoryResult.error, server);
    }
}
