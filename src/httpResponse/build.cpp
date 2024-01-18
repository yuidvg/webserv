#include "build.hpp"

static HttpResponse responseToValidRequest(const HttpRequest request, const Server server)
{
    // TODO: server.rootの部分でエラーが出るため、locations[0].rootに変更
    const std::string fullPath = server.locations[0].root + request.uri;
    const utils::FileContentResult openedFile = utils::content(fullPath);
    return openedFile.success
               ? HttpResponse(SUCCESS,
                              Headers(utils::contentType(fullPath), utils::toString(openedFile.value.length())),
                              openedFile.value)
               : HttpResponse(BAD_REQUEST, Headers(), "");
}

HttpResponse response(const ParseRequestResult requestResult, const Server server)
{
    return requestResult.success ? responseToValidRequest(requestResult.value, server) : requestResult.error;
}

std::string makeResponseMessage(const HttpResponse response)
{
    std::string text = SERVER_PROTOCOL + " " + std::to_string(response.statusCode) + CRLF;
    for (std::map<std::string, std::string>::const_iterator it = response.headers.begin(); it != response.headers.end();
         ++it)
        text += it->first + ": " + it->second + CRLF;
    text += CRLF;
    text += response.body;
    return text;
}
