#include "../config/serverConfig.hpp"

unsigned int lengthOfPrefixMatch(const std::string string, const std::string pattern)
{
	unsigned int i = 0;
	while (i < string.length() && i < pattern.length() && string[i] == pattern[i])
		i++;
	return i;
}


RouteConfig matchedRoute(const std::string uri, const std::vector<RouteConfig> routes)
{
	for (unsigned int i = 0; i < routes.size(); i++)
	{
		if (uri.find(routes[i].) == 0)
			return routes[i];
	}
	return RouteConfig();
}
