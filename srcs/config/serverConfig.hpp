#ifndef SERVER_CONFIG_HPP
#define SERVER_CONFIG_HPP

#include <map>
#include <string>
#include <vector>

struct RouteConfig
{
	const std::vector<const std::string> acceptedMethods;
	const HttpRedirect redirect;
	const std::string rootPath;
	const bool directoryListing;
	const bool acceptFiles;
	const std::string defaultFilePath;
	const std::string uploadPath;
	const std::map<const std::string, const std::string> cgiExtensions;
};

struct DefaultErrorPage
{
	const std::vector<const unsigned int> statuses;
	const std::string path;
};

struct HttpRedirect
{
	const unsigned int status;
	const std::string path;
};

struct ServerConfig
{
	const std::string host;
	const unsigned int port;
	const std::vector<const std::string> serverNames;
	const std::vector<const DefaultErrorPage> defaultErrorPages;
	const unsigned int clientBodySizeLimit;
	const std::map<const std::string, const RouteConfig> routes;
};

#endif
