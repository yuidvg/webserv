#include "../autoindex/all.hpp"
#include "../config/path.hpp"
#include "build.hpp"

HttpResponse conductGet(const HttpRequest &request, const Location &location)
{
    const std::string targetResourcePath = comply(request.target, location);
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
                           ? HttpResponse(SUCCESS,
                                          Headers(utils::contentType(".html"),
                                                  utils::toString(directoryListHtmlResult.value.length())),
                                          directoryListHtmlResult.value)
                           : BAD_REQUEST_RESPONSE;
            }
            else
            {
                return BAD_REQUEST_RESPONSE;
            }
        }
        else // when targetResourcePath is assumed to be a file.
        {
            const utils::FileContentResult fileContentResult = utils::content(targetResourcePath);
            return fileContentResult.success ? HttpResponse(SUCCESS,
                                                            Headers(utils::contentType(targetResourcePath),
                                                                    utils::toString(fileContentResult.value.length())),
                                                            fileContentResult.value)
                                             : BAD_REQUEST_RESPONSE;
        }
    }
    else
    {
        return isDirectoryResult.error;
    }
}
