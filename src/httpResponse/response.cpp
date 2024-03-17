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
} // namespace

ImmidiateResponse retrieveImmidiateResponse(const HttpRequest &request, const Client &client,
                                            const ErrorPages &errorPages)
{
    const Location location = CONFIG.getServer(request.host, client.serverPort).getLocation(request.target);

    if (isMethodAllowed(request, location))
    {
        const std::string resolvedPath = resolvePath(request.target, location);
        const Uri uri = segmentUri(resolvedPath, location.cgiExtension);
        if (uri.scriptPath.size() > 0)
        {
            return ImmidiateResponse::Left(retrieveCgiRequest(request, client, uri));
        }
        else
        {
            if (!location.redirect.empty())
            {
                return ImmidiateResponse::Right(redirectResponse(location.redirect));
            }
            if (request.method == "GET")
                return ImmidiateResponse::Right(conductGet(uri, location, errorPages));
            else if (request.method == "POST")
                return ImmidiateResponse::Right(conductPost(request, location, errorPages));
            else if (request.method == "DELETE")
                return ImmidiateResponse::Right(conductDelete(request.target, errorPages));
            else
                return ImmidiateResponse::Right(METHOD_NOT_ALLOWED_RESPONSE("GET, POST, DELETE"));
        }
    }
    else
    {
        return ImmidiateResponse::Right(METHOD_NOT_ALLOWED_RESPONSE(utils::join(location.allowMethods, ", ")));
    }
}

std::string responseText(const HttpResponse &response)
{
    std::vector<std::string> headers;
    const std::string statusLine = SERVER_PROTOCOL + " " + std::to_string(response.statusCode);
    headers.push_back(statusLine);
    if (response.contentType.size() > 0)
    {
        headers.push_back("Content-Type: " + response.contentType);
    }
    if (response.body.size() >= 0)
    {
        headers.push_back("Content-Length: " + std::to_string(response.body.size()));
    }
    if (response.location.size() > 0)
    {
        headers.push_back("Location: " + response.location);
    }
    if (response.allow.size() > 0)
    {
        headers.push_back("Allow: " + response.allow);
    }
    const std::string headersText = utils::join(headers, CRLF);

    std::vector<std::string> blocks;
    blocks.push_back(headersText);
    blocks.push_back("");
    blocks.push_back(response.body);
    return utils::join(blocks, CRLF);
}
