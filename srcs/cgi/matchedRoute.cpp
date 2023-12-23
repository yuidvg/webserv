#include "../config/serverConfig.hpp"

RouteConfig matchedRoute(const std::string uri, const std::vector<RouteConfig> routes)
{
	for (unsigned int i = 0; i < routes.size(); i++)
	{
		if (uri.find(routes[i].) != std::string::npos)
			return routes[i];
	}
	return RouteConfig();
}