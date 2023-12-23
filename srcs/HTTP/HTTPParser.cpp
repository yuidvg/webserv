#include "HTTPParser.hpp"

bool	isLineTooLong(const std::string &line)
{
	if (line.length() > MAX_LEN)
		return (true);
	return (false);
}

bool	checkMethod(std::string method, int &error_code)
{
	std::string	allowed_methods[] = {"GET", "POST", "DELETE"}; // 本来はconfigから取得する？
	for (int i = 0; i < 3; i++)
	{
		if (method == allowed_methods[i])
			break ;
		if (i == 2)
		{
			error_code = 405;
			return (false);
		}
	}
	return (true);
}

bool	checkTarget(std::string uri, int &error_code)
{
	if (uri.find(':') != std::string::npos && uri.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
	{
		error_code = HTTP_STATUS_BAD_REQUEST; // 400
		return (false);
	}
	return (true);
}

bool	checkVersion(std::string version, int &error_code)
{
	if (version != "HTTP/1.1")
	{
		error_code = 505;
		return (false);
	}
	return (true);
}

ParseResult	parseHTTPRequestLine(std::string &httpRequest)
{
	std::string		request_line;
	std::string		method, uri, version;

	std::cout << "====parseRequestLine====" << std::endl; // debug

	while (customGetLine(httpRequest, request_line) && request_line.empty())
		; // 空行を読み飛ばす

	// 有効なリクエストラインがない場合
	if (request_line.empty())
		return (ParseResult::Err(HTTP_STATUS_BAD_REQUEST)); // 400
	if (isLineTooLong(request_line) == true)
		return (ParseResult::Err(HTTP_STATUS_REQUEST_URI_TOO_LONG)); // 414

	std::cout << "request_line: " << request_line << std::endl; // debug

	std::istringstream	line(request_line);
	if (!(line >> method >> uri >> version) || !line.eof()) // メソッドとターゲット、バージョンに分けて格納する
		return (ParseResult::Err(HTTP_STATUS_BAD_REQUEST)); // 400

	/* エラーチェック */
	int	error_code = 200;
	if (checkMethod(method, error_code) == false || checkTarget(uri, error_code) == false || checkVersion(version, error_code) == false)
		return (ParseResult::Err(error_code));

	ParsedRequest	parsed_data(method, uri, version, std::map<std::string, std::string>(), "");

	return (ParseResult::Ok(parsed_data));
}

// ParseResult	parseHTTPHeader(std::string &httpRequest)
// {
// 	std::string			line;

// 	std::cout << "====parseHeader====" << std::endl; // debug

// 	while (customGetLine(httpRequest, line))
// 	{
// 		if (line.empty())
// 			break ;
// 		if (isLineTooLong(line) == true)
// 			return (FAILURE);
// 		if (std::isspace(line[0]))
// 		{
// 			error_code = HTTP_STATUS_BAD_REQUEST; // 400
// 			return (FAILURE);
// 		}

// 		// ヘッダーの解析
// 		std::string			key, value;
// 		std::istringstream	iss(line);

// 		std::getline(iss, key, ':');
// 		std::getline(iss, value);
// 		trim(value); // valueの前後の空白を削除する

// 		if (key.empty() || std::isspace(*(key.end() - 1)) || value.empty())
// 		{
// 			error_code = HTTP_STATUS_BAD_REQUEST; // 400
// 			return (FAILURE);
// 		}

// 		_header[key] = value;
// 		std::cout << "[key]: " << key << ", [value]: " << _header[key] << std::endl; // debug
// 	}
// 	if (!line.empty() || _header.empty())
// 	{
// 		error_code = HTTP_STATUS_BAD_REQUEST; // 400
// 		return (FAILURE);
// 	}
// 	return (SUCCESS);
// }

// ParseResult	parseHTTPBody(std::string &httpRequest)
// {
// 	std::string			line;
// 	std::istringstream	iss(httpRequest);

// 	std::cout << "====parseBody====" << std::endl; // debug

// 	if (httpRequest.empty())
// 		std::cout << "httpRequestなし!!" << std::endl; // debug
// 	else
// 		std::cout << "httpRequest: " << httpRequest << std::endl; // debug

// 	if (_header["Content-Length"].empty() || _header["Transfer-Encoding"].empty())
// 	{
// 		error_code = HTTP_STATUS_BAD_REQUEST; // 400
// 		return (FAILURE);
// 	}

// 	if (!_header["Content-Length"].empty() && (stoi(_header["Content-Length"]) > MAX_LEN))
// 	{
// 		error_code = 413;
// 		return (FAILURE);
// 	}

// 	while (customGetLine(httpRequest, line))
// 	{
// 		if (line.empty())
// 			break ;
// 		if (isLineTooLong(line) == true)
// 			return (FAILURE);
// 		if (std::isspace(line[0]))
// 		{
// 			error_code = HTTP_STATUS_BAD_REQUEST; // 400
// 			return (FAILURE);
// 		}
// 	}
// 	return (SUCCESS);
// }

// void	executeParse(std::string &data)
// {
// 	std::string		request_line;

// 	error_code = 200;
// 	if (parseHTTPRequestLine(data) == FAILURE)
// 		return;

// 	if (parseHTTPHeader(data) == FAILURE)
// 		return;

// 	if (parseHTTPBody(data) == FAILURE)
// 		return;
// }
