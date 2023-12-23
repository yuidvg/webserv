#include "HTTPParser.hpp"

static bool	isLineTooLong(const std::string &line)
{
	if (line.length() > MAX_LEN)
		return (true);
	return (false);
}

static bool	checkMethod(std::string method, int &error_code)
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

static bool	checkTarget(std::string uri, int &error_code)
{
	if (uri.find(':') != std::string::npos && uri.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
	{
		error_code = HTTP_STATUS_BAD_REQUEST; // 400
		return (false);
	}
	return (true);
}

static bool	checkVersion(std::string version, int &error_code)
{
	if (version != "HTTP/1.1")
	{
		error_code = 505;
		return (false);
	}
	return (true);
}

ParseResult	parseHTTPRequest(std::string &httpRequest)
{
	std::cout << "====parseRequestLine====" << std::endl; // debug
	std::string		line;
	std::string		method, uri, version;

	while (customGetLine(httpRequest, line) && line.empty())
		; // 空行を読み飛ばす

	// 有効なリクエストラインがない場合
	if (line.empty())
		return (ParseResult::Err(HTTP_STATUS_BAD_REQUEST)); // 400
	if (isLineTooLong(line) == true)
		return (ParseResult::Err(HTTP_STATUS_REQUEST_URI_TOO_LONG)); // 414

	std::cout << "request_line: " << line << std::endl; // debug

	std::istringstream	request_line(line);
	if (!(request_line >> method >> uri >> version) || !request_line.eof()) // メソッドとターゲット、バージョンに分けて格納する
		return (ParseResult::Err(HTTP_STATUS_BAD_REQUEST)); // 400

	/* エラーチェック */
	int	error_code = 200;
	if (checkMethod(method, error_code) == false || checkTarget(uri, error_code) == false || checkVersion(version, error_code) == false)
		return (ParseResult::Err(error_code));


	std::cout << "====parseHeader====" << std::endl; // debug
	std::map<std::string, std::string>	header;


	while (customGetLine(httpRequest, line))
	{
		if (line.empty())
			break ;
		if (isLineTooLong(line) == true)
			return (ParseResult::Err(HTTP_STATUS_REQUEST_URI_TOO_LONG)); // 414
		if (std::isspace(line[0]))
			return (ParseResult::Err(HTTP_STATUS_BAD_REQUEST)); // 400

		// ヘッダーの解析
		std::string			key, value;
		std::istringstream	header_line(line);

		std::getline(header_line, key, ':');
		std::getline(header_line, value);
		trim(value); // valueの前後の空白を削除する

		if (key.empty() || std::isspace(*(key.end() - 1)) || value.empty())
			return (ParseResult::Err(HTTP_STATUS_BAD_REQUEST)); // 400

		header[key] = value;
		std::cout << "[key]: " << key << ", [value]: " << header[key] << std::endl; // debug
	}
	if (!line.empty() || header.empty())
		return (ParseResult::Err(HTTP_STATUS_BAD_REQUEST)); // 400


	std::cout << "====parseBody====" << std::endl; // debug

	if (httpRequest.empty())
		std::cout << "httpRequestなし!!" << std::endl; // debug
	else
		std::cout << "httpRequest: " << httpRequest << std::endl; // debug

	if ((header["Content-Length"].empty() || header["Transfer-Encoding"].empty()) && !httpRequest.empty())
		return (ParseResult::Err(HTTP_STATUS_BAD_REQUEST)); // 400

	if (!header["Content-Length"].empty() && (stoi(header["Content-Length"]) > MAX_LEN))
		return (ParseResult::Err(413));

	while (customGetLine(httpRequest, line))
	{
		if (line.empty())
			break ;
		if (isLineTooLong(line) == true)
			return (ParseResult::Err(HTTP_STATUS_REQUEST_URI_TOO_LONG)); // 414
		if (std::isspace(line[0]))
			return (ParseResult::Err(HTTP_STATUS_BAD_REQUEST)); // 400
	}
	ParsedRequest	parsed_data(method, uri, version, header, httpRequest);
	return (ParseResult::Ok(parsed_data));
}
