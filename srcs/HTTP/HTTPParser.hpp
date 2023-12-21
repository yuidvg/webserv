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

struct ParsedRequest
{
	const std::string							method;
	const std::string							version;
	const std::string							url;
	const std::map<std::string, std::string>	header;
	const std::string							body;
};

// 成功したら構造体、失敗したらエラーコードを返すresultを作成する
template <typename T, typename E>
struct Result
{
	Result(T const &ok) : t(tag::ok)
	{
		;
	}
};

bool	isLineTooLong(const std::string &line);
bool	checkMethod(void);
bool	checkVersion(void);
bool	checkTarget(void);
Result	parseHTTPHeader(std::string &httpRequest);
Result	parseHTTPBody(std::string &httpRequest);
Result	parseHTTPRequestLine(std::string &httpRequest);

#endif
