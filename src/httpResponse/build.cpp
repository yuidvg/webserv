#include "build.hpp"

static HttpResponse responseToValidRequest(const HttpRequest request, const Server server)
{
    const Location location = utils::matchedLocation(request.uri, server.locations);
    const std::string rootedPath = utils::rooted(request.uri, location);
    const std::string indexedPath = utils::indexed(rootedPath, location);
    const utils::FileContentResult fileContentResult = utils::content(rootedPath);
    return fileContentResult.success
               ? HttpResponse(SUCCESS,
                              Headers(utils::contentType(rootedPath), utils::toString(fileContentResult.value.length())),
                              fileContentResult.value)
               : HttpResponse(BAD_REQUEST, Headers(), "");
}

HttpResponse response(const ParseRequestResult requestResult, const Server server)
{
    return requestResult.success ? responseToValidRequest(requestResult.value, server) : requestResult.error;
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
