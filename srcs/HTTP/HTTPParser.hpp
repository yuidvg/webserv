#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include <cstring>
# include <unistd.h>
# include <iostream>
# include <map>

class HTTPParser
{
	private:
		enum state
		{
			e_request_line,
			e_header,
			e_body,
			e_done
		};
		state								_state;
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

		void	executeParse(std::string &data);
};

#endif
