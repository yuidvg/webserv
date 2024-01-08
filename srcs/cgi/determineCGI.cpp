#include "../HTTPRequest/HTTPParser.hpp"
#include "../config/structure_config.hpp"
#include "../utils/utils.hpp"

bool shouldUseCGI(const ParsedRequest request, const Server server)
{
    const Route route = utils::matchedRoute(request.uri, server.routes);
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
