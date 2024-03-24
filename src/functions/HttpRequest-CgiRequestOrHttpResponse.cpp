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
StringMap getCgiEnvs(const HttpRequest &httpRequest)
{
    const Uri uri = segment(httpRequest);
    StringMap env;
    env.insert(std::make_pair("AUTH_TYPE", authType(httpRequest)));
    env.insert(std::make_pair("CONTENT_LENGTH", utils::itoa(httpRequest.body.size())));
    env.insert(std::make_pair("CONTENT_TYPE", utils::value(httpRequest.headers, std::string("content-type"))));
    env.insert(std::make_pair("GATEWAY_INTERFACE", GATEWAY_INTERFACE));
    env.insert(std::make_pair("PATH_INFO", uri.extraPath));
    env.insert(std::make_pair(
        "PATH_TRANSLATED",
        uri.extraPath.size() > 0
            ? resolvePath(uri.extraPath,
                          CONFIG.getServer(httpRequest.host, httpRequest.serverPort).getLocation(httpRequest.target))
            : ""));
    env.insert(std::make_pair("QUERY_STRING", uri.queryString));
    env.insert(std::make_pair("REMOTE_ADDR", httpRequest.clientIp));
    env.insert(std::make_pair("REQUEST_METHOD", httpRequest.method));
    env.insert(std::make_pair("SCRIPT_NAME", uri.scriptPath));
    env.insert(std::make_pair("SERVER_NAME", httpRequest.host));
    env.insert(std::make_pair("SERVER_PORT", httpRequest.serverPort));
    env.insert(std::make_pair("SERVER_PROTOCOL", SERVER_PROTOCOL));
    env.insert(std::make_pair("SERVER_SOFTWARE", SERVER_SOFTWARE));
    return env;
}
} // namespace

CgiRequestOrHttpResponse processHttpRequest(const HttpRequest &httpRequest)
{
    const Location location = getLocation(httpRequest);

    if (isMethodAllowed(httpRequest, location))
    {
        const std::string resolvedPath = resolvePath(httpRequest.target, location);
        if (segment(httpRequest).scriptPath.size() > 0)
        {
            StringMap cgiEnvs = getCgiEnvs(httpRequest);
            ConnectedUnixSocketResult cgiProcessResult = createCgiProcess(cgiEnvs, segment(httpRequest).scriptPath);
            if (cgiProcessResult.success)
            {
                CGI_HTTP_REQUESTS.insert(std::make_pair(cgiProcessResult.value.descriptor, httpRequest));
                return CgiRequestOrHttpResponse::Left(CgiRequest(cgiProcessResult.value.descriptor, cgiEnvs,
                                                                 segment(httpRequest).scriptPath, httpRequest.body));
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
