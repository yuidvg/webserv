#include ".hpp"
#include "../cgiRequest/.hpp"
#include "../httpRequestAndConfig/.hpp"

namespace
{
bool isMethodAllowed(const HttpRequest &request, const Location &location)
{
    for (std::vector<std::string>::const_iterator it = location.allowMethods.begin(); it != location.allowMethods.end();
         ++it)
        if (*it == request.method)
            return true;
    return false;
}

HttpResponse responseToValidRequest(const HttpRequest &request, const Socket &socket)
{
    const Server server = CONFIG.getServer(request.host, socket.port);
    const Location location = server.getLocation(request.target);

    if (isMethodAllowed(request, location))
    {
        const ScriptUri scriptUri = retrieveScriptUri(request.target, location.cgiExtension);
        if (scriptUri.scriptPath.size() > 0)
        {
            return executeCgi(request, socket, scriptUri);
        }
        else
        {
            if (!location.redirect.empty())
                return redirectResponse(location.redirect);
            else if (request.method == "GET")
                return conductGet(request, location);
            else if (request.method == "POST")
                return conductPost(request, location);
            else if (request.method == "DELETE")
                return conductDelete(request, location);
            else
                return METHOD_NOT_ALLOWED_RESPONSE("GET, POST, DELETE");
        }
    }
    else
    {
        return METHOD_NOT_ALLOWED_RESPONSE(utils::join(location.allowMethods, ", "));
    }
}
} // namespace

HttpResponse response(const ParseRequestResult &requestResult, const Socket &socket)
{
    const HttpRequest request = requestResult.value;
    const Server server = CONFIG.getServer(request.host, socket.port);
    const HttpResponse httpResponse =
        responseToValidRequest(requestResult.value, Socket(socket.descriptor, socket.port));
    if (httpResponse.statusCode == BAD_REQUEST || httpResponse.statusCode == SERVER_ERROR)
    {
        return provideErrorResponse(httpResponse, server);
    }
    return (httpResponse);
}

std::string responseText(const HttpResponse &response)
{
    std::string text = SERVER_PROTOCOL + " " + std::to_string(response.statusCode) + CRLF;
    for (std::map<std::string, std::string>::const_iterator it = response.headers.begin(); it != response.headers.end();
         ++it)
        text += it->first + ": " + it->second + CRLF;
    text += CRLF;
    text += response.body;
    return text;
}
