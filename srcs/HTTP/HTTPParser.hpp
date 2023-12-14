#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include <cstring>
# include <unistd.h>
# include <iostream>

class HTTPPARSER
{
	private:
		std::string	_requestline;
		std::string	_method;
		std::string	_version;
		std::string	_header;
		std::string	_body;

	public:
		HTTPPARSER(char *request);
		HTTPPARSER(const HTTPPARSER &src);
		HTTPPARSER	&operator=(const HTTPPARSER &src);
		~HTTPPARSER(void);

		std::string	getRequestline(void) const;
		// std::string	getMethod(void) const;
		std::string	getHeader(void) const;
		std::string	getBody(void) const;

		void	parseHTTPRequest(char *request);
};

#endif
