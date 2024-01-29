#include ".hpp"

ParseRequestResult parseHttpRequest(HttpRequestText &httpRequestText)
{
    std::cout << "======requestLine=======\n";

    std::string httpRequest = httpRequestText.getText();
    std::istringstream requestTextStream(httpRequest);

    const ParseRequestLineResult parseRequestLineResult = parseHttpRequestLine(requestTextStream);
    if (!parseRequestLineResult.success)
        return ParseRequestResult::Error(HttpResponse(parseRequestLineResult.error));

    std::cout << "======Header=======\n";
    const ParseHeaderResult headersResult = parseHttpHeaders(requestTextStream);
    if (!headersResult.success)
    {
        std::cout << "Failed" << std::endl;
        return ParseRequestResult::Error(HttpResponse(headersResult.error));
    }

    std::cout << "headersResult.value: \n";

    const Headers headers = headersResult.value;

    std::cout << "======Body=======\n";
    const ParseBodyResult body = parseHttpBody(requestTextStream, headers);
    if (!body.success)
        return ParseRequestResult::Error(HttpResponse(body.error));

    const RequestLine requestLine = parseRequestLineResult.value;
    const HttpRequest result(requestLine.method, requestLine.target, requestLine.version, headers, body.value,
                             httpRequestText.getHostName().value);
    return ParseRequestResult::Success(result);
}

static bool isLineTooLong(const std::string &line)
{
    if (line.length() > MAX_LEN)
        return true;
    return false;
}

static int getRequestLineStatusCode(const RequestLine requestLine)
{
    if (requestLine.target.find(':') != std::string::npos &&
        requestLine.target.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
        return BAD_REQUEST;

    if (requestLine.version != SERVER_PROTOCOL)
        return BAD_REQUEST;

    return SUCCESS;
}

static GetRequestLineResult getRequestLine(std::istringstream &requestTextStream)
{
    std::string method, target, version;
    std::string line;

    while (std::getline(requestTextStream, line))
        ;

    // 有効なリクエストラインがない場合
    if (requestTextStream.eof())
        return (GetRequestLineResult::Error(BAD_REQUEST));
    if (isLineTooLong(line))
        return GetRequestLineResult::Error(BAD_REQUEST);

    std::istringstream requestLine(line);
    if (!(requestLine >> method >> target >> version) ||
        !requestLine.eof()) // メソッドとターゲット、バージョンに分けて格納する
        return GetRequestLineResult::Error(BAD_REQUEST);

    RequestLine requestLineElements = {method, target, version};
    return GetRequestLineResult::Success(requestLineElements);
}

ParseRequestLineResult parseHttpRequestLine(std::istringstream &requestTextStream)
{
    const GetRequestLineResult getRequestLineResult = getRequestLine(requestTextStream);
    if (!getRequestLineResult.success)
        return ParseRequestLineResult::Error(getRequestLineResult.error);

    /* エラーチェック */
    const int statusCode = getRequestLineStatusCode(getRequestLineResult.value);

    return (statusCode != SUCCESS) ? ParseRequestLineResult::Error(statusCode)
                                   : ParseRequestLineResult::Success(getRequestLineResult.value);
}

ParseHeaderResult parseHttpHeaders(std::istringstream &requestTextStream)
{
    std::string line;
    Headers headers;

    while (std::getline(requestTextStream, line))
    {
        if (isLineTooLong(line))
            return ParseHeaderResult::Error(BAD_REQUEST);
        if (std::isspace(line[0]))
            return ParseHeaderResult::Error(BAD_REQUEST);

        // ヘッダーの解析
        std::string key, value;
        std::istringstream headerLine(line);

        if (!std::getline(headerLine, key, ':') || !std::getline(headerLine, value))
            return ParseHeaderResult::Error(BAD_REQUEST);

        utils::trim(value);

        if (key.empty() || std::isspace(*(key.end() - 1)) || value.empty())
            return ParseHeaderResult::Error(BAD_REQUEST);

        headers[utils::lowerCase(key)] = value;
        // std::cout << "text:\n" << httpRequestText.getText() << std::endl;
    }

    if (!line.empty() || headers.empty())
        return ParseHeaderResult::Error(BAD_REQUEST);

    // headersの中身を表示
    std::cout << "headers: " << std::endl;
    for (Headers::iterator it = headers.begin(); it != headers.end(); it++)
    {
        std::cout << it->first << "=>" << it->second << "." << std::endl;
    }

    return ParseHeaderResult::Success(headers);
}

ParseBodyResult parseHttpBody(std::istringstream &requestTextStream, const Headers &headers)
{
    std::string line;
    std::string body;

    if (headers.find("transfer-encoding") != headers.end())
    {
        if (headers.at("transfer-encoding") != "chunked")
            return ParseBodyResult::Error(BAD_REQUEST);
        while (std::getline(requestTextStream, line))
        {
            if (isLineTooLong(line))
                return ParseBodyResult::Error(BAD_REQUEST);
            if (line == "0")
                break;
            body += line;
        }
    }
    else if (headers.find("content-length") != headers.end())
    {
        const int contentLength = std::stoi(headers.at("content-length"));
        if (contentLength < 0)
            return ParseBodyResult::Error(BAD_REQUEST);
        for (int i = 0; i < contentLength; i++)
        {
            if (!std::getline(requestTextStream, line))
                return ParseBodyResult::Error(BAD_REQUEST);
            if (isLineTooLong(line))
                return ParseBodyResult::Error(BAD_REQUEST);
            body += line;
        }
    }
    else
    {
        return ParseBodyResult::Success(body);
    }
    std::cout << "body: \n" << body << std::endl;
    return ParseBodyResult::Success(body);
}
