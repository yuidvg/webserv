#include "../HTTP/HTTPParser.hpp"
#include "matchedRoute.cpp"

const bool shouldUseCGI(const ParsedRequest request, const Server server)
{
    const Route route = matchedRoute(request.uri, server.routes);
    const std::vector<std::string> extensions = route.cgiExtensions;
    for (unsigned int i = 0; i < extensions.size(); i++)
    {
        const std::string extension = extensions[i];
        const std::string uriExtension = request.uri.substr(request.uri.find_last_of(".") + 1);
        if (extension == uriExtension)
            return true;
    }
    return false;
}
