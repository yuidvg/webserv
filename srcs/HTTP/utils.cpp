#include "utils.hpp"

std::string	getLine(std::string &data)
{
	std::string		line;
	size_t			pos;

	pos = data.find("\n");
	if (pos == std::string::npos)
		return (line);
	line = data.substr(0, pos);
	data.erase(0, pos + 1);
	return (line);
}
