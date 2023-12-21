#include "utils.hpp"

bool	customGetLine(std::string &data, std::string &line)
{
	std::istringstream	iss(data);
	size_t				read_length;

	if (std::getline(iss, line))
	{
		read_length = line.length();
		if (data[line.length()] != '\n')
		{
			std::cout << "異常" << std::endl; // debug
			return (false);
		}
		read_length++;
		data.erase(0, read_length);
		return (true);
	}
	return (false);
}
