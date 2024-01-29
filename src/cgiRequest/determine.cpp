#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

namespace cgi
{
bool shouldUse(const HttpRequest &request, const Location &location)
{
    const std::string uriExtension = request.target.substr(request.target.find_last_of(".") + 1);
    return uriExtension == location.cgiExtension;
}

} // namespace cgi
