#ifndef UTILS_HPP
# define UTILS_HPP

# include <cstring>
# include <sstream>
# include <iostream>

bool		customGetLine(std::string &data, std::string &line);
std::string	&trim(std::string &str);
std::string	toLower(std::string str);
bool		isNumber(const std::string &str);

#endif
