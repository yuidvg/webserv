#ifndef HTTPPARSER_HPP
# define HTTPPARSER_HPP

# include <cstring>
# include <unistd.h>
# include <iostream>
# include <sstream>
# include <map>

# include "utils.hpp"
# include "Result.hpp"

# define SUCCESS 0
# define FAILURE 1

# define MAX_LEN 8192

# define HTTP_STATUS_BAD_REQUEST 400
# define HTTP_STATUS_REQUEST_URI_TOO_LONG 414

struct ParsedRequest
{
	const std::string							method;
	const std::string							uri;
	const std::string							version;
	const std::map<std::string, std::string>	header;
	const std::string							body;

	ParsedRequest(const std::string &m, const std::string &u, const std::string &v, const std::map<std::string, std::string> &h, const std::string &b) 
			: method(m), uri(u), version(v), header(h), body(b) {}
};

typedef Result<ParsedRequest, int> ParseResult;

ParseResult	parseHTTPRequest(std::string &httpRequest);

#endif
