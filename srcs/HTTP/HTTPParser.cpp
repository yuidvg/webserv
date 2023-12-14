#include "HTTPParser.hpp"

HTTPParser::HTTPParser(const std::string &request)
{
	parse(request);
}

HTTPParser::HTTPParser(const HTTPParser &src) {}

HTTPParser	&HTTPParser::operator=(const HTTPParser &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return (*this);
}

HTTPParser::~HTTPParser(void) {}

void	HTTPParser::parse(const std::string &data)
{
	
}
