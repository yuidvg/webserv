#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include <cstring>
# include <unistd.h>
# include <iostream>
# include <sstream>
# include <map>

# include "utils.hpp"

# define SUCCESS 0
# define FAILURE 1

# define HTTP_STATUS_BAD_REQUEST 400
# define HTTP_STATUS_REQUEST_URI_TOO_LONG 414


class HTTPParser
{
	private:
		static const int					MAX_LEN = 8192;

		std::string							_method;
		std::string							_version;
		std::string							_url;
		std::map<std::string, std::string>	_header;
		std::string							_body;
		int									_error_code;

		int		parseHTTPRequestLine(std::string &data);
		int		parseHTTPHeader(std::string &data);
		int		parseHTTPBody(std::string &data);

		bool	checkMethod(void);
		bool	checkVersion(void);
		bool	checkTarget(void);

		bool	isLineTooLong(const std::string &line);

	public:
		HTTPParser(std::string &request);
		HTTPParser(const HTTPParser &src);
		HTTPParser	&operator=(const HTTPParser &rhs);
		~HTTPParser(void);

		std::string	getMethod(void) const;
		std::string	getTarget(void) const;
		std::string	getVersion(void) const;
		int			getErrorCode(void) const;

		void	executeParse(std::string &data);
};

#endif
