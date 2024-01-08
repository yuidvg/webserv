#include "../HttpRequest/RequestParser.hpp"
#include "../utils/utils.hpp"

bool shouldUseCgi(const ParsedRequest request, const Server server)
{
    const Location location = utils::matchedLocation(request.uri, server.locations);
    const std::string uriExtension = request.uri.substr(request.uri.find_last_of(".") + 1);
    return uriExtension == location.cgiExtension;
}
