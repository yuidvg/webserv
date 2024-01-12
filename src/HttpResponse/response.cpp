#include "response.hpp"

static HttpResponse responseToValidRequest(const ParsedRequest request, const Server server)
{
    const std::string fullPath = server.root + request.uri;
    const utils::FileContentResult openedFile = utils::content(fullPath);
    return openedFile.success
               ? HttpResponse(SUCCESS,
                              Headers(utils::contentType(fullPath), utils::toString(openedFile.value.length())),
                              openedFile.value)
               : HttpResponse(BAD_REQUEST, Headers(), "");
}

HttpResponse response(const ParseRequestResult requestResult, const Server server)
{
    return requestResult.success ? responseToValidRequest(requestResult.value, server) : requestResult.value;
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

HttpResponse::HttpResponse() : statusCode(SUCCESS), headers(Headers()), body("")
{
}

HttpResponse::HttpResponse(const unsigned int statusCode, const Headers headers = Headers(),
                           const std::string body = "")
    : statusCode(statusCode), headers(headers), body(body)
{
}
