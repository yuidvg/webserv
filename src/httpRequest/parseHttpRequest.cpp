#include ".hpp"

ParseRequestResult parseHttpRequest(HttpRequestText &httpRequestText, const Server &server)
{
    std::string httpRequest = httpRequestText.getText();
    std::istringstream requestTextStream(httpRequest);

    const ParseRequestLineResult parseRequestLineResult = parseHttpRequestLine(requestTextStream);
    if (!parseRequestLineResult.success)
        return ParseRequestResult::Error(HttpResponse(parseRequestLineResult.error));

    const ParseHeaderResult headersResult = parseHttpHeaders(requestTextStream);
    if (!headersResult.success)
    {
        std::cout << "Failed" << std::endl;
        return ParseRequestResult::Error(HttpResponse(headersResult.error));
    }

    const Headers headers = headersResult.value;

    const ParseBodyResult body = parseHttpBody(requestTextStream, headers, server);
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

static std::string getlineCustom(std::istringstream &requestTextStream)
{
    std::string line;
    std::getline(requestTextStream, line);
    // lineの後ろの\rを削除
    if (line[line.length() - 1] == '\r')
        line.erase(line.length() - 1);
    return line;
}

static GetRequestLineResult getRequestLine(std::istringstream &requestTextStream)
{
    std::string method, target, version;
    std::string line;

    while ((line = getlineCustom(requestTextStream)).empty())
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

    while (!(line = getlineCustom(requestTextStream)).empty())
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

    return ParseHeaderResult::Success(headers);
}

ParseBodyResult parseHttpBody(std::istringstream &requestTextStream, const Headers &headers, const Server &server)
{
    std::string line;
    std::string body((std::istreambuf_iterator<char>(requestTextStream)), std::istreambuf_iterator<char>());

    if (headers.find("transfer-encoding") != headers.end())
    {
        if (headers.at("transfer-encoding") != "chunked")
            return ParseBodyResult::Error(BAD_REQUEST);
        // if (body.length() > clientMaxBodySize)
        //     return ParseBodyResult::Error(BAD_REQUEST);
    }
    else if (headers.find("content-length") != headers.end())
    {
        if (!utils::isNumber(headers.at("content-length")))
            return ParseBodyResult::Error(BAD_REQUEST);
        const size_t bodySize = static_cast<size_t>(std::stoi(headers.at("content-length")));
        if (bodySize < 0 || bodySize > server.clientMaxBodySize)
            return ParseBodyResult::Error(BAD_REQUEST);
        if (body.length() != server.clientMaxBodySize)
            return ParseBodyResult::Error(BAD_REQUEST);
    }
    else
    {
        // bodyがない場合
        return ParseBodyResult::Success(body);
    }
    return ParseBodyResult::Success(body);
}
