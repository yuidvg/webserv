#include ".hpp"

ParseRequestResult parseHttpRequest(HttpRequestText &httpRequestText)
{
    std::cout << "======requestLine=======\n";

    const ParseRequestLineResult parseRequestLineResult = parseHttpRequestLine(httpRequestText);
    if (!parseRequestLineResult.success)
        return ParseRequestResult::Error(HttpResponse(parseRequestLineResult.error));

    std::cout << "======Header=======\n";
    const ParseHeaderResult headersResult = parseHttpHeaders(httpRequestText);
    if (!headersResult.success)
    {
        std::cout << "Failed" << std::endl;
        return ParseRequestResult::Error(HttpResponse(headersResult.error));
    }

    std::cout << "headersResult.value: \n";

    const Headers headers = headersResult.value;

    std::cout << "======Body=======\n";
    const ParseBodyResult body = parseHttpBody(httpRequestText, headers);
    if (!body.success)
        return ParseRequestResult::Error(HttpResponse(body.error));

    const RequestLine requestLine = parseRequestLineResult.value;
    const HttpRequest result(requestLine.method, requestLine.target, requestLine.version, headers, body.value,
                             httpRequestText.getHostName());
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

static GetRequestLineResult getRequestLine(HttpRequestText &httpRequestText)
{
    std::string method, target, version;
    std::string line;

    line = httpRequestText.readLine();
    while (line.empty())
        line = httpRequestText.readLine();

    // 有効なリクエストラインがない場合
    if (httpRequestText.eof())
        return (GetRequestLineResult::Error(BAD_REQUEST));
    if (isLineTooLong(line))
        return GetRequestLineResult::Error(BAD_REQUEST);

    std::istringstream requestLine(line);
    if (!(requestLine >> method >> target >> version) ||
        !requestLine.eof()) // メソッドとターゲット、バージョンに分けて格納する
        return GetRequestLineResult::Error(BAD_REQUEST);

    std::cout << "line: " << line << std::endl;

    std::cout << "text=> \n" << httpRequestText.getText() << std::endl;

    RequestLine requestLineElements = {method, target, version};
    return GetRequestLineResult::Success(requestLineElements);
}

ParseRequestLineResult parseHttpRequestLine(HttpRequestText &httpRequest)
{
    const GetRequestLineResult getRequestLineResult = getRequestLine(httpRequest);
    if (!getRequestLineResult.success)
        return ParseRequestLineResult::Error(getRequestLineResult.error);

    /* エラーチェック */
    const int statusCode = getRequestLineStatusCode(getRequestLineResult.value);

    return (statusCode != SUCCESS) ? ParseRequestLineResult::Error(statusCode)
                                   : ParseRequestLineResult::Success(getRequestLineResult.value);
}

ParseHeaderResult parseHttpHeaders(HttpRequestText &httpRequestText)
{
    std::string line;
    Headers headers;

    while (!(line = httpRequestText.readLine()).empty())
    {
        std::cout << "line: " << line << std::endl;
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
    }

    if (!line.empty() || headers.empty())
        return ParseHeaderResult::Error(BAD_REQUEST);

    return ParseHeaderResult::Success(headers);
}

ParseBodyResult parseHttpBody(HttpRequestText &httpRequestText, const Headers &headers)
{
    std::string line;
    std::string body;

    if (headers.find("transfer-encoding") != headers.end())
    {
        if (headers.at("transfer-encoding") != "chunked")
            return ParseBodyResult::Error(BAD_REQUEST);
        body = httpRequestText.getText();
    }
    else if (headers.find("content-length") != headers.end())
    {
        const unsigned int contentLength = std::stoi(headers.at("content-length"));
        if (contentLength > MAX_LEN || httpRequestText.getText().length() != contentLength)
            return ParseBodyResult::Error(BAD_REQUEST);
    }
    std::cout << "body: \n" << body << std::endl;
    return ParseBodyResult::Success(body);
}
