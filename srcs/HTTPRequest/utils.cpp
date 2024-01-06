#include "utils.hpp"

bool	customGetLine(std::string &data, std::string &line)
{
	std::istringstream	iss(data);
	size_t				read_length;

	if (std::getline(iss, line))
	{
		read_length = line.length();
		if (data[line.length()] != '\n')
			return (false);
		read_length++;
		data.erase(0, read_length);
		return (true);
	}
	return (false);
}

std::string	&trim(std::string &str)
{
	while (std::isspace(str[0]))
		str.erase(0, 1);
	while (std::isspace(str[str.length() - 1]))
		str.erase(str.length() - 1, 1);
	return (str);
}

std::string	toLower(std::string str)
{
	std::transform(&str[0], &str[0] + str.size(), &str[0], ::tolower);
	return (str);
}

bool	isNumber(const std::string &str)
{
	for (int i = 0; str[i]; i++)
	{
		if (!std::isdigit(str[i]))
			return (false);
	}
	return (true);
}
