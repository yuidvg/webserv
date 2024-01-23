#include ".hpp"
#include "../httpRequestAndConfig/.hpp"

ParseRequestResult parseHttpRequest(std::istream &httpRequest, const Servers &servers, const Sd &sd)
{
    const ParseRequestLineResult parseRequestLineResult = parseHttpRequestLine(httpRequest);
    if (!parseRequestLineResult.success)
        return ParseRequestResult::Error(HttpResponse(parseRequestLineResult.error));

    const ParseHeaderResult headersResult = parseHttpHeaders(httpRequest);
    if (!headersResult.success)
        return ParseRequestResult::Error(HttpResponse(headersResult.error));

    const Headers headers = headersResult.value;

    /* hostの取得 */
    Headers::const_iterator it;
    if ((it = headers.find("host")) == headers.end())
        return ParseRequestResult::Error(HttpResponse(BAD_REQUEST));
    const std::string host = it->second;

    /* サーバの選択 */
    const MatchedServerResult matchedServerResult = matchedServer(host, servers, sd);
    if (!matchedServerResult.success)
        return ParseRequestResult::Error(HttpResponse(matchedServerResult.error));
    const Server server = matchedServerResult.value;

    const ParseBodyResult body = parseHttpBody(httpRequest, headers, server);
    if (!body.success)
        return ParseRequestResult::Error(HttpResponse(body.error));

    const RequestLine requestLine = parseRequestLineResult.value;
    const HttpRequest result(requestLine.method, requestLine.target, requestLine.version, headers, body.value, host);
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

static std::string getMessageLine(std::istream &stream)
{
    std::string line;
    char c;

    while (stream.get(c))
    {
        if (c == '\r')
        {
            if (stream.peek() == '\n')
            {
                stream.get();
                break;
            }
            else
                c = '\n';
        }
        line += c;
    }
    return line;
}

static GetRequestLineResult getRequestLine(std::istream &httpRequest)
{
    std::string method, target, version;
    std::string line;

    while ((line = getMessageLine(httpRequest)) == "\r")
        ;

    // 有効なリクエストラインがない場合
    if (httpRequest.eof())
        return (GetRequestLineResult::Error(BAD_REQUEST));
    if (isLineTooLong(line))
        return GetRequestLineResult::Error(BAD_REQUEST);

    std::istringstream requestLine(line);
    if (!(requestLine >> method >> target >> version) ||
        !requestLine.eof()) // メソッドとターゲット、バージョンに分けて格納する
        return GetRequestLineResult::Error(BAD_REQUEST);

    RequestLine requestLineData = {method, target, version};
    return GetRequestLineResult::Success(requestLineData);
}

ParseRequestLineResult parseHttpRequestLine(std::istream &httpRequest)
{
    const GetRequestLineResult getRequestLineResult = getRequestLine(httpRequest);
    if (!getRequestLineResult.success)
        return ParseRequestLineResult::Error(getRequestLineResult.error);

    /* エラーチェック */
    const int statusCode = getRequestLineStatusCode(getRequestLineResult.value);

    return (statusCode != SUCCESS) ? ParseRequestLineResult::Error(statusCode)
                                   : ParseRequestLineResult::Success(getRequestLineResult.value);
}

ParseHeaderResult parseHttpHeaders(std::istream &httpRequest)
{
    std::string line;
    Headers headers;

    while ((line = getMessageLine(httpRequest)) != "\r")
    {
        if (line.empty())
            break;
        if (isLineTooLong(line))
            return ParseHeaderResult::Error(BAD_REQUEST);
        if (std::isspace(line[0]))
            return ParseHeaderResult::Error(BAD_REQUEST);

        // ヘッダーの解析
        std::string key, value;
        std::istringstream headerLine(line);

        std::getline(headerLine, key, ':');
        std::getline(headerLine, value);
        utils::trim(value);

        if (key.empty() || std::isspace(*(key.end() - 1)) || value.empty())
            return ParseHeaderResult::Error(BAD_REQUEST);

        headers[utils::lowerCase(key)] = value;
    }
    if (!line.empty() || headers.empty())
        return ParseHeaderResult::Error(BAD_REQUEST);

    return ParseHeaderResult::Success(headers);
}

ParseBodyResult parsePlainBody(std::istream &httpRequest, const Headers &headers)
{
    std::string line;

    Headers::const_iterator it = headers.find("content-length");
    if (it == headers.end() || !utils::isNumber(it->second))
        return ParseBodyResult::Error(BAD_REQUEST);

    const size_t contentLength = std::stoul(it->second);
    if (contentLength > MAX_LEN)
        return ParseBodyResult::Error(BAD_REQUEST);

    std::string body(contentLength, '\0');
    if (httpRequest.read(&body[0], contentLength))
        return ParseBodyResult::Error(BAD_REQUEST);

    return ParseBodyResult::Success(body);
}

ParseBodyResult parseChunkedBody(std::istream &httpRequest, const Headers &headers, const Server &server)
{
    std::string line;

    Headers::const_iterator it = headers.find("transfer-encoding");
    if (it->second != "chunked")
        return ParseBodyResult::Error(HttpResponse(BAD_REQUEST));

    std::string body = "";
    while ((line = getMessageLine(httpRequest)) != "\r" && body.length() < server.clientMaxBodySize)
    {
        if (isLineTooLong(line))
            return ParseBodyResult::Error(HttpResponse(BAD_REQUEST));

        std::istringstream chunkSizeLine(line);
        int chunkSize;
        if (!(chunkSizeLine >> std::hex >> chunkSize) || !chunkSizeLine.eof())
            return ParseBodyResult::Error(HttpResponse(BAD_REQUEST));

        // チャンクのサイズを読み取り、0でない限り続ける
        if (chunkSize == 0)
            break;

        char *buffer = new char[chunkSize];
        httpRequest.read(buffer, chunkSize);
        body.append(buffer, chunkSize);
        delete[] buffer;

        // チャンクの末尾のCRLFを読み飛ばす
        std::getline(httpRequest, line);
    }
    line += EOF;

    return ParseBodyResult::Success(body);
}

ParseBodyResult parseHttpBody(std::istream &httpRequest, const Headers &headers, const Server &server)
{
    std::string line;

    if (headers.find("transfer-encoding") != headers.end())
        return parseChunkedBody(httpRequest, headers, server);
    else if (headers.find("content-length") != headers.end())
        return parsePlainBody(httpRequest, headers);
    else
        return ParseBodyResult::Success("");
}
