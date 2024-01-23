#include "../config/find.hpp"
#include "cgi.hpp"

namespace cgi
{
bool shouldUse(const HttpRequest request, const Server server)
{
    const Location location = matchedLocation(request.target, server.locations);
    const std::string uriExtension = request.target.substr(request.target.find_last_of(".") + 1);
    return uriExtension == location.cgiExtension;
}

} // namespace cgi
