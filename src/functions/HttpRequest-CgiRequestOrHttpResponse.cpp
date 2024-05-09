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
                Option<Socket> cgiSocket = createCgiProcess(httpRequest, rootedScriptPath);
                if (cgiSocket)
                {
                    CGI_HTTP_REQUESTS.insert(std::make_pair(*cgiSocket, httpRequest));
                    return Option<HttpResponse>();
                }
                else
                    return getErrorHttpResponse(httpRequest, SERVER_ERROR);
            }
            else
            {
                if (!location.redirect.empty())
                    return getRedirectHttpResponse(httpRequest, location.redirect);
                else if (httpRequest.method == "GET")
                    return conductGet(httpRequest, resolvedPath);
                else if (httpRequest.method == "POST")
                    return conductPost(httpRequest);
                else if (httpRequest.method == "DELETE")
                    return conductDelete(httpRequest);
                else
                    return getMethodNotAllowedResponse(httpRequest, "GET, POST, DELETE");
            }
        }
        else
        {
            return getMethodNotAllowedResponse(httpRequest, utils::join(location.allowMethods, ", "));
        }
    }
    else
    {
        return getErrorHttpResponse(httpRequest, BAD_REQUEST);
    }
}
