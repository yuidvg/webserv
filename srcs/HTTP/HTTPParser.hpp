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
	const std::string							version;
	const std::string							url;
	const std::map<std::string, std::string>	header;
	const std::string							body;
};

typedef Result<ParsedRequest, int> ParseResult;

bool		isLineTooLong(const std::string &line);
bool		checkMethod(void);
bool		checkVersion(void);
bool		checkTarget(void);
ParseResult	parseHTTPHeader(std::string &httpRequest);
ParseResult	parseHTTPBody(std::string &httpRequest);
ParseResult	parseHTTPRequestLine(std::string &httpRequest);

#endif
