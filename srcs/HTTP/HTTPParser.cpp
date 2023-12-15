#include "HTTPParser.hpp"

HTTPParser::HTTPParser(const std::string &request)
{
	parse(request);
}

HTTPParser::HTTPParser(const HTTPParser &src) {}

HTTPParser	&HTTPParser::operator=(const HTTPParser &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return (*this);
}

HTTPParser::~HTTPParser(void) {}

static std::string	getRequestLine(std::string &data)
{
	std::string		request_line;
	size_t			pos;

	pos = data.find("\r\n");
	if (pos == std::string::npos)
		return (request_line);
	request_line = data.substr(0, pos);
	data.erase(0, pos + 2);
	return (request_line);
}

void	HTTPParser::executeParse(std::string &data)
{
	std::string		request_line;

	request_line = getRequestLine(data);
	/* メソッドとターゲット、バージョンを格納する(error処理も) */
	/* ヘッダーを格納する(error処理も) */
	/* ボディを格納する(error処理も) */
}
