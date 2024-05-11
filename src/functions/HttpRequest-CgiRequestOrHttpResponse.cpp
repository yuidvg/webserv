#include "../all.hpp"

namespace
{
bool isMethodAllowed(const HttpRequest &request, const Location &location)
{
    for (std::vector< std::string >::const_iterator it = location.allowMethods.begin();
         it != location.allowMethods.end(); ++it)
        if (*it == request.method)
            return true;
    return false;
}

} // namespace

std::pair< Option< HttpResponse >, Option< EventData > > processHttpRequest(const HttpRequest &httpRequest)
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
                Option< Socket > cgiSocket = createCgiProcess(httpRequest, rootedScriptPath);
                if (cgiSocket)
                {
                    CGI_HTTP_REQUESTS.insert(std::make_pair(*cgiSocket, httpRequest));
                    return std::make_pair(Option< HttpResponse >(), Option< EventData >());
                }
                else
                    return std::make_pair(getErrorHttpResponse(httpRequest, SERVER_ERROR), Option< EventData >());
            }
            else
            {
                if (!location.redirect.empty())
                    return std::make_pair(getRedirectHttpResponse(httpRequest, location.redirect),
                                          Option< EventData >());
                else if (httpRequest.method == "GET")
                    return std::make_pair(conductGet(httpRequest, resolvedPath), Option< EventData >());
                else if (httpRequest.method == "POST")
                {
                    std::pair< HttpResponse, Option< EventData > > postResponse = conductPost(httpRequest);
                    return std::make_pair(postResponse.first, postResponse.second);
                }
                else if (httpRequest.method == "DELETE")
                    return std::make_pair(conductDelete(httpRequest), Option< EventData >());
                else
                    return std::make_pair(getMethodNotAllowedResponse(httpRequest, "GET, POST, DELETE"),
                                          Option< EventData >());
            }
        }
        else
        {
            return std::make_pair(getMethodNotAllowedResponse(httpRequest, utils::join(location.allowMethods, ", ")),
                                  Option< EventData >());
        }
    }
    else
    {
        return std::make_pair(getErrorHttpResponse(httpRequest, BAD_REQUEST), Option< EventData >());
    }
}
