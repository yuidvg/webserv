#include "../autoindex/all.hpp"
#include "build.hpp"

HttpResponse conductGet(const HttpRequest &request, const Location &location)
{
    const std::string rootedPath = utils::root(request.target, location);
    const std::string indexedPath = utils::index(rootedPath, location);
    const IsDirectoryResult isDirectoryResult = utils::isDirectory(indexedPath);
    if (isDirectoryResult.success)
    {
        const bool isDirectory = isDirectoryResult.value;
        if (isDirectory)
        {
            if (location.autoindex)
            {
                const DirectoryListHtmlResult directoryListHtmlResult = directoryListHtml(indexedPath);
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
        else // when indexedPath is assumed to be a file.
        {
            const utils::FileContentResult fileContentResult = utils::content(indexedPath);
            return fileContentResult.success ? HttpResponse(SUCCESS,
                                                            Headers(utils::contentType(indexedPath),
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
