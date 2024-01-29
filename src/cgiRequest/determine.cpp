#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

namespace cgi
{
bool shouldUse(const HttpRequest request, const Server server)
{
    const Location location = matchedLocation(request.target, server.locations);
    const std::string uriExtension = request.target.substr(request.target.find_last_of(".") + 1);
    return uriExtension == location.cgiExtension;
}

// std::string cgiPath(const HttpRequest &request, const Location &location)
// {
//     return ;
// }
} // namespace cgi
