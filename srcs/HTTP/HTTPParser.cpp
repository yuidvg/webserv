#include "HTTPParser.hpp"

HTTPParser::HTTPParser(std::string &request)
{
	executeParse(request);
}

HTTPParser::HTTPParser(const HTTPParser &src)
{
	(*this) = src;
}

HTTPParser	&HTTPParser::operator=(const HTTPParser &rhs)
{
	if (this != &rhs)
	{
		// copy
	}
	return (*this);
}

HTTPParser::~HTTPParser(void) {}

std::string	HTTPParser::getMethod(void) const
{
	return (_method);
}

std::string	HTTPParser::getTarget(void) const
{
	return (_url);
}

std::string	HTTPParser::getVersion(void) const
{
	return (_version);
}

int			HTTPParser::getErrorCode(void) const
{
	return (_error_code);
}

int	HTTPParser::parseRequestLine(std::string &data)
{
	std::string	request_line;

	std::cout << "====parseRequestLine====" << std::endl; // debug
	std::cout << "[data]\n" << data << std::endl; // debug
	request_line = getLine(data);
	std::cout << "request_line: " << request_line << std::endl; // debug

	while (request_line.empty())
		request_line = getLine(data);

	if (request_line.length() > 8192)
	{
		_error_code = 414;
		return (FAILURE);
	}
	/* メソッドとターゲット、バージョンを格納する */
	if (countWord(request_line) != 3)
	{
		_error_code = 400;
		return (FAILURE);
	}
	std::istringstream	iss(request_line);
	iss >> _method >> _url >> _version;

	/* method error処理(checkMethod関数など作成する) */
	std::string	allowed_methods[] = {"GET", "POST", "DELETE"}; // configから取得する
	for (int i = 0; i < 3; i++)
	{
		if (_method == allowed_methods[i])
			break ;
		if (i == 2)
		{
			_error_code = 405;
			return (FAILURE);
		}
	}

	/* target error処理 */
	// if (_url.find(':')) // connectは対応しない
	// {
	// 	_error_code = 400;
	// 	return (FAILURE);
	// }

	/* version error処理 */
	if (_version != "HTTP/1.1")
	{
		_error_code = 505;
		return (FAILURE);
	}

	return (SUCCESS);
}

int	HTTPParser::parseHeader(std::string &data)
{
	std::string		line;

	std::cout << "====parseHeader====" << std::endl; // debug
	/* ヘッダーを格納する(error処理も) */
	std::cout << "[data]\n" << data << std::endl; // debug
	while (true)
	{
		line = getLine(data);
		if (line.empty())
			break ;
		if (line.length() > 8192)
		{
			_error_code = 414;
			return (FAILURE);
		}
		// 空白で始まる行は不正として扱う
		if (std::isspace(line[0]))
		{
			_error_code = 400;
			return (FAILURE);
		}

		/* _headerに格納する */
		std::string			key, value;
		std::istringstream	iss(line);
		std::getline(iss, key, ':'); // keyの最後に空白がある時のERROR処理を追加する必要あり
		std::getline(iss, value);
		value = value.substr(1); // 先頭の空白を削除

		_header[key] = value;
		std::cout << "[key]: " << key << ", [value]: " << _header[key] << std::endl; // debug
	}
	return (SUCCESS);
}

int	HTTPParser::parseBody(std::string &data)
{
	std::string		line;

	std::cout << "====parseBody====" << std::endl; // debug
	/* ボディを格納する(error処理も) */
	std::cout << "[data]\n" << data << std::endl; // debug
	// if (_header["Content-Length"] == "" || _header["Transfer-Encoding"] == "")
	// {
	// 	std::cout << "bodyなし" << std::endl; // debug
	// 	return (SUCCESS);
	// }
	std::cout << "test: " << _header["Content-Length"] << std::endl; // debug
	while (true)
	{
		line = getLine(data);
		if (line.empty())
			break ;
		std::cout << stoi(_header["Content-Length"]) << std::endl; // debug
		// if (line.length() > stoi(_header["Content-Length"]))
		// {
		// 	_error_code = 414;
		// 	return (FAILURE);
		// }
		if (line[0] == ' ')
		{
			_error_code = 400;
			return (FAILURE);
		}

		/* _bodyに格納する */
		_body += line + "\n";
	}
	return (SUCCESS);
}

void	HTTPParser::executeParse(std::string &data)
{
	std::string		request_line;

	_error_code = 200;
	if (parseRequestLine(data) == FAILURE)
		return;

	if (parseHeader(data) == FAILURE)
		return;

	/* ボディを格納する(error処理も) */
	if (parseBody(data) == FAILURE)
		return;
}
