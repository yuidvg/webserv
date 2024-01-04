#include "../HTTP/HTTPParser.hpp"
#include "matchedRoute.cpp"

bool shouldUseCGI(const ParsedRequest request, ServerConfig config)
{
    Location location = matchedLocation(request.uri, config.locations);
	if (location.cgi_path.empty())
}
