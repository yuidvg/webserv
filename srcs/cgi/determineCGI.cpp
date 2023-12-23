#include "../HTTP/HTTPParser.hpp"
#include "../config/serverConfig.hpp"

bool shouldUseCGI(const ParsedRequest request, ServerConfig config)
{
	if (request.uri.find() != std::string::npos)
		return true;

}

