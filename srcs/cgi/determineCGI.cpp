#include "../HTTPRequest/HTTPParser.hpp"
#include "../config/structure_config.hpp"

bool shouldUseCgi(const ParsedRequest request, const Server server)
{
    const Location location = utils::matchedLocation(request.uri, server.locations);
    const std::vector<std::string> extensions = location.;
    for (unsigned int i = 0; i < extensions.size(); i++)
    {
        const std::string extension = extensions[i];
        const std::string uriExtension = request.uri.substr(request.uri.find_last_of(".") + 1);
        if (extension == uriExtension)
            return true;
    }
    return false;
}
