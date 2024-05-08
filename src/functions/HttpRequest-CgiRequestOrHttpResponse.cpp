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
            ? utils::resolvePath(
                  uri.extraPath,
                  CONFIG.getServer(httpRequest.host, httpRequest.socket.serverPort).getLocation(httpRequest.target))
            : ""));
    env.insert(std::make_pair("QUERY_STRING", uri.queryString));
    env.insert(std::make_pair("REMOTE_ADDR", httpRequest.socket.clientIp));
    env.insert(std::make_pair("REQUEST_METHOD", httpRequest.method));
    env.insert(std::make_pair("SCRIPT_NAME", uri.scriptPath));
    env.insert(std::make_pair("SERVER_NAME", httpRequest.host));
    env.insert(std::make_pair("SERVER_PORT", std::to_string(httpRequest.socket.serverPort)));
    env.insert(std::make_pair("SERVER_PROTOCOL", SERVER_PROTOCOL));
    env.insert(std::make_pair("SERVER_SOFTWARE", SERVER_SOFTWARE));
    return env;
}

} // namespace

Option<HttpResponse> processHttpRequest(const HttpRequest &httpRequest)
{
    if (httpRequest.host.length() > 0)
    {
        const Location location = getLocation(httpRequest);

        if (isMethodAllowed(httpRequest, location))
        {
            const std::string resolvedPath = utils::resolvePath(httpRequest.target, location);
            const std::string scriptPath = segment(httpRequest).scriptPath;
            if (scriptPath.size() > 0)
            {
                const std::string rootedScriptPath = utils::root(scriptPath, location);
                StringMap cgiEnvs = getCgiEnvs(httpRequest);
                Option<Socket> cgiSocket = createCgiProcess(cgiEnvs, rootedScriptPath);
                if (cgiSocket)
                {
                    return Option<HttpResponse>();
                }
                else
                    return Option<HttpResponse>(getErrorHttpResponse(httpRequest, SERVER_ERROR));
            }
            else
            {
                if (!location.redirect.empty())
                    return Option<HttpResponse>(getRedirectHttpResponse(httpRequest, location.redirect));
                else if (httpRequest.method == "GET")
                    return Option<HttpResponse>(conductGet(httpRequest, resolvedPath));
                else if (httpRequest.method == "POST")
                    return Option<HttpResponse>(conductPost(httpRequest));
                else if (httpRequest.method == "DELETE")
                    return Option<HttpResponse>(conductDelete(httpRequest));
                else
                    return Option<HttpResponse>(getMethodNotAllowedResponse(httpRequest, "GET, POST, DELETE"));
            }
        }
        else
        {
            return Option<HttpResponse>(
                getMethodNotAllowedResponse(httpRequest, utils::join(location.allowMethods, ", ")));
        }
    }
    else
    {
        return Option<HttpResponse>(getErrorHttpResponse(httpRequest, BAD_REQUEST));
    }
}
