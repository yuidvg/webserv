#include "../httpRequestAndConfig/.hpp"
#include ".hpp"
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
HttpResponse responseToValidRequest(const HttpRequest &request, const Server &server)
{
    const Location location = matchedLocation(request.target, server.locations);
    if (isMethodAllowed(request, location))
    {
        if (request.method == "GET")
            return conductGet(request, location);
        else if (request.method == "POST")
            return conductPost(request, location);
        else if (request.method == "DELETE")
            return conductDelete(request, location);
        else
            return METHOD_NOT_ALLOWED_RESPONSE("GET, POST, DELETE");
    }
    else
    {
        return METHOD_NOT_ALLOWED_RESPONSE(utils::join(location.allowMethods, ", "));
    }
}
} // namespace

HttpResponse response(const ParseRequestResult &requestResult, const Socket &sd, const Servers &servers)
{
    if (requestResult.success)
    {
        const HttpRequest request = requestResult.value;
        const MatchedServerResult serverResult = matchedServer(request.host, servers, sd);
        if (serverResult.success)
        {
            const Server server = serverResult.value;
            return responseToValidRequest(requestResult.value, server);
        }
        else
        {

            return BAD_REQUEST_RESPONSE;
        }
    }
    else
    {
        return BAD_REQUEST_RESPONSE;
    }
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
