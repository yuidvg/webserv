#include "../all.hpp"

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
CgiRequest getCgiRequest(const HttpRequest &httpRequest)
{
    const Uri uri = segmentize(httpRequest);
    std::map<std::string, std::string> env;
    env["AUTH_TYPE"] = authType(httpRequest);
    env["CONTENT_LENGTH"] = utils::itoa(httpRequest.body.size());
    env["CONTENT_TYPE"] = utils::value(httpRequest.headers, std::string("content-type"));
    env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
    env["PATH_INFO"] = uri.extraPath;
    env["PATH_TRANSLATED"] =
        uri.extraPath.size() > 0
            ? resolvePath(uri.extraPath,
                          CONFIG.getServer(httpRequest.host, httpRequest.serverPort).getLocation(httpRequest.target))
            : "";
    env["QUERY_STRING"] = uri.queryString;
    env["REMOTE_ADDR"] = httpRequest.clientIp;
    env["REQUEST_METHOD"] = httpRequest.method;
    env["SCRIPT_NAME"] = uri.scriptPath;
    env["SERVER_NAME"] = httpRequest.host;
    env["SERVER_PORT"] = httpRequest.serverPort;
    env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;
    env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;

    return CgiRequest(env, uri.scriptPath, httpRequest.body);
}
} // namespace

CgiRequestOrHttpResponse processHttpRequest(const HttpRequest &httpRequest)
{
    const Location location = getLocation(httpRequest);

    if (isMethodAllowed(httpRequest, location))
    {
        const std::string resolvedPath = resolvePath(httpRequest.target, location);
        if (segmentize(httpRequest).scriptPath.size() > 0)
        {
            CgiRequest cgiRequest = getCgiRequest(httpRequest);
            ConnectedUnixSocketResult cgiProcessResult = createCgiProcess(cgiRequest);
            if (cgiProcessResult.success)
            {
                CGI_HTTP_REQUESTS.insert(std::make_pair(cgiProcessResult.value.descriptor, httpRequest));
                return CgiRequestOrHttpResponse::Left(cgiRequest);
            }
            else
                return CgiRequestOrHttpResponse::Right(getErrorResponse(httpRequest, SERVER_ERROR));
        }
        else
        {
            if (!location.redirect.empty())
                return CgiRequestOrHttpResponse::Right(getRedirectResponse(httpRequest, location.redirect));
            else if (httpRequest.method == "GET")
                return CgiRequestOrHttpResponse::Right(conductGet(httpRequest));
            else if (httpRequest.method == "POST")
                return CgiRequestOrHttpResponse::Right(conductPost(httpRequest));
            else if (httpRequest.method == "DELETE")
                return CgiRequestOrHttpResponse::Right(conductDelete(httpRequest));
            else
                return CgiRequestOrHttpResponse::Right(getMethodNotAllowedResponse(httpRequest, "GET, POST, DELETE"));
        }
    }
    else
    {
        return CgiRequestOrHttpResponse::Right(
            getMethodNotAllowedResponse(httpRequest, utils::join(location.allowMethods, ", ")));
    }
}
