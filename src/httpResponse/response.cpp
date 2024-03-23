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
std::string authType(const HttpRequest &request)
{
    const std::string authorization = utils::value(request.headers, std::string("authorization"));
    const std::vector<std::string> tokens = utils::split(authorization, " ");
    return tokens.size() > 0 ? tokens[0] : "";
}
CgiRequest getCgiRequest(const HttpRequest &request, const Uri &uri)
{
    std::map<std::string, std::string> env;
    env["AUTH_TYPE"] = authType(request);
    env["CONTENT_LENGTH"] = utils::itoa(request.body.size());
    env["CONTENT_TYPE"] = utils::value(request.headers, std::string("content-type"));
    env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
    env["PATH_INFO"] = uri.extraPath;
    env["PATH_TRANSLATED"] =
        uri.extraPath.size() > 0
            ? resolvePath(uri.extraPath, CONFIG.getServer(request.host, request.serverPort).getLocation(request.target))
            : "";
    env["QUERY_STRING"] = uri.queryString;
    env["REMOTE_ADDR"] = request.clientIp;
    env["REQUEST_METHOD"] = request.method;
    env["SCRIPT_NAME"] = uri.scriptPath;
    env["SERVER_NAME"] = request.host;
    env["SERVER_PORT"] = request.serverPort;
    env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;
    env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;

    return CgiRequest(env, uri.scriptPath, request.body);
}

} // namespace

CgiRequestOrHttpResponse getCgiRequestOrHttpResponse(const HttpRequest &request)
{
    const Location location = CONFIG.getServer(request.host, request.serverPort).getLocation(request.target);

    if (isMethodAllowed(request, location))
    {
        const std::string resolvedPath = resolvePath(request.target, location);
        const Uri uri = segmentUri(resolvedPath, location.cgiExtension);
        if (uri.scriptPath.size() > 0)
        {
            return CgiRequestOrHttpResponse::Left(getCgiRequest(request, uri));
        }
        else
        {
            if (!location.redirect.empty())
            {
                return CgiRequestOrHttpResponse::Right(redirectResponse(location.redirect));
            }
            if (request.method == "GET")
                return CgiRequestOrHttpResponse::Right(conductGet(uri, location));
            else if (request.method == "POST")
                return CgiRequestOrHttpResponse::Right(conductPost(request, location));
            else if (request.method == "DELETE")
                return CgiRequestOrHttpResponse::Right(conductDelete(request.target));
            else
                return CgiRequestOrHttpResponse::Right(METHOD_NOT_ALLOWED_RESPONSE("GET, POST, DELETE"));
        }
    }
    else
    {
        return CgiRequestOrHttpResponse::Right(METHOD_NOT_ALLOWED_RESPONSE(utils::join(location.allowMethods, ", ")));
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
