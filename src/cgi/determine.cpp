#include "cgi.hpp"

namespace cgi
{

bool shouldUse(const HttpRequest request, const Server server)
{
    const Location location = utils::matchedLocation(request.uri, server.locations);
    const std::string uriExtension = request.uri.substr(request.uri.find_last_of(".") + 1);
    return uriExtension == location.cgiExtension;
}

} // namespace cgi