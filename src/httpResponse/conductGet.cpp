#include ".hpp"
#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"

HttpResponse conductGet(const Uri &uri, const Location &location)
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
                           : BAD_REQUEST_RESPONSE;
            }
            else if (location.index.size() > 0)
            {
                const std::string indexPath = uri.extraPath + "/" + location.index;
                const FileContentResult fileContentResult = utils::fileContent(indexPath);
                return fileContentResult.success
                           ? HttpResponse(SUCCESS, fileContentResult.value, utils::contentType(indexPath))
                           : BAD_REQUEST_RESPONSE;
            }
            else
            {
                return BAD_REQUEST_RESPONSE;
            }
        }
        else // when uri.extraPath is assumed to be a file.
        {
            const FileContentResult fileContentResult = utils::fileContent(uri.extraPath);
            return fileContentResult.success
                       ? HttpResponse(SUCCESS, fileContentResult.value, utils::contentType(uri.extraPath))
                       : BAD_REQUEST_RESPONSE;
        }
    }
    else
    {
        return isDirectoryResult.error;
    }
}
