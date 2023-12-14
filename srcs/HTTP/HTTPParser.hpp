#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include <cstring>
# include <unistd.h>
# include <iostream>
# include <map>

class HTTPParser
{
	private:
		enum State
		{
			REQUESTLINE,
			HEADER,
			BODY,
			DONE
		};
		State								_state;
		std::string							_method;
		std::string							_url;
		std::map<std::string, std::string>	_header;
		std::string							_body;
		int									_error_code;

	public:
		HTTPParser(const std::string &request);
		HTTPParser(const HTTPParser &src);
		HTTPParser	&operator=(const HTTPParser &rhs);
		~HTTPParser(void);

		void	parse(const std::string &data);
};

#endif
