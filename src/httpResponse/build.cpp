#include "build.hpp"
#include "../autoindex/all.hpp"

static HttpResponse responseToValidRequest(const HttpRequest request, const Server server)
{

    const Location location = utils::matchedLocation(request.uri, server.locations);
    const std::string rootedPath = utils::root(request.uri, location);
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

HttpResponse response(const ParseRequestResult requestResult, const Sd sd, const Servers servers)
{
    return requestResult.success ? responseToValidRequest(requestResult.value, utils::matchedServer(servers, sd))
                                 : requestResult.error;
}

std::string responseText(const HttpResponse response)
{
    std::string text = SERVER_PROTOCOL + " " + std::to_string(response.statusCode) + CRLF;
    for (std::map<std::string, std::string>::const_iterator it = response.headers.begin(); it != response.headers.end();
         ++it)
        text += it->first + ": " + it->second + CRLF;
    text += CRLF;
    text += response.body;
    return text;
}
