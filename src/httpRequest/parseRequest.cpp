#include "parseRequest.hpp"

static bool isLineTooLong(const std::string &line)
{
    if (line.length() > MAX_LEN)
        return (true);
    return (false);
}

static bool checkMethod(const std::string &method)
{
    std::vector<std::string> allowedMethods;
    allowedMethods.push_back("GET");
    allowedMethods.push_back("POST");
    allowedMethods.push_back("DELETE");
    for (int i = 0; i < 3; i++)
    {
        if (method == allowedMethods[i])
            return (true);
    }
    return (false);
}

static bool checkRequestLine(std::string &method, std::string &uri, std::string &version, int &errorCode,
                             const Server &server)
{
    if (uri.find(':') != std::string::npos && uri.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
    {
        errorCode = BAD_REQUEST;
        return (false);
    }

    (void)server;
    if (!checkMethod(method))
    {
        errorCode = BAD_REQUEST;
        return (false);
    }
    if (version != "Http/1.1")
    {
        errorCode = BAD_REQUEST;
        return (false);
    }
    return (true);
}

ParseRequestLineResult parseHttpRequestLine(std::istream &httpRequest, const Server &server)
{
    std::string line;
    std::string method, uri, version;

    while (std::getline(httpRequest, line) && line.empty())
        ; // 空行を読み飛ばす

    // 有効なリクエストラインがない場合
    if (line.empty())
        return (ParseRequestLineResult::Error(BAD_REQUEST));
    if (isLineTooLong(line) == true)
        return (ParseRequestLineResult::Error(BAD_REQUEST));

    std::istringstream requestLine(line);
    if (!(requestLine >> method >> uri >> version) ||
        !requestLine.eof()) // メソッドとターゲット、バージョンに分けて格納する
        return (ParseRequestLineResult::Error(BAD_REQUEST));

    /* エラーチェック */
    int errorCode = SUCCESS;
    if (!checkRequestLine(method, uri, version, errorCode, server))
        return (ParseRequestLineResult::Error(errorCode));

    RequestLine requestLineData = {method, uri, version};
    return (ParseRequestLineResult::Success(requestLineData));
}

ParseHeaderResult parseHttpHeaders(std::istream &httpRequest)
{
    std::string line;
    Headers headers;

    while (std::getline(httpRequest, line))
    {
        if (line.empty())
            break;
        if (isLineTooLong(line) == true)
            return (ParseHeaderResult::Error(BAD_REQUEST));
        if (std::isspace(line[0]))
            return (ParseHeaderResult::Error(BAD_REQUEST));

        // ヘッダーの解析
        std::string key, value;
        std::istringstream headerLine(line);

        std::getline(headerLine, key, ':');
        std::getline(headerLine, value);
        utils::trim(value); // valueの前後の空白を削除する

        if (key.empty() || std::isspace(*(key.end() - 1)) || value.empty())
            return (ParseHeaderResult::Error(BAD_REQUEST));

        headers[utils::lowerCase(key)] = value; // keyを小文字に変換して格納する
        std::cout << "[key]: " << key << ", [value]: " << headers[utils::lowerCase(key)] << std::endl; // debug
    }
    if (!line.empty() || headers.empty())
        return (ParseHeaderResult::Error(BAD_REQUEST));

    return (ParseHeaderResult::Success(headers));
}

ParseBodyResult parseChunkedBody(std::istream &httpRequest, std::map<std::string, std::string> &headers)
{
    std::cout << "====parseChunkedBody====" << std::endl; // debug
    std::string line;

    if (headers["transfer-encoding"] != "chunked")
        return (ParseBodyResult::Error(HttpResponse(BAD_REQUEST)));

    std::string body;
    while (std::getline(httpRequest, line))
    {
        // if (line.empty())
        // 	break ;
        if (isLineTooLong(line) == true)
            return (ParseBodyResult::Error(HttpResponse(BAD_REQUEST)));

        std::istringstream chunkSizeLine(line);
        int chunkSize;
        if (!(chunkSizeLine >> std::hex >> chunkSize) || !chunkSizeLine.eof())
            return (ParseBodyResult::Error(HttpResponse(BAD_REQUEST)));

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
    std::cout << "Processed body: " << body << std::endl;

    return (ParseBodyResult::Success(body));
}

ParseBodyResult parsePlainBody(std::istream &httpRequest, std::map<std::string, std::string> &headers)
{
    std::cout << "====parsePlainBody====" << std::endl; // debug
    std::string line;

    if (headers["content-length"].empty() || !utils::isNumber(headers["content-length"]))
        return (ParseBodyResult::Error(BAD_REQUEST));

    size_t contentLength = std::stoul(headers["content-length"]);
    if (contentLength > MAX_LEN)
        return (ParseBodyResult::Error(BAD_REQUEST));

    std::cout << "contentLength: " << contentLength << std::endl; // debug
    std::string body(contentLength, '\0');
    if (httpRequest.read(&body[0], contentLength))
        return (ParseBodyResult::Error(BAD_REQUEST));

    return (ParseBodyResult::Success(body));
}

ParseBodyResult parseHttpBody(std::istream &httpRequest, std::map<std::string, std::string> &headers)
{
    std::cout << "====parseBody====" << std::endl; // debug
    std::string line;

    if (headers.find("transfer-encoding") != headers.end())
        return (parseChunkedBody(httpRequest, headers));
    else if (headers.find("content-length") != headers.end())
        return (parsePlainBody(httpRequest, headers));
    else
        return (ParseBodyResult::Success(""));
}

ParseRequestResult parseHttpRequest(std::istream &httpRequest, const Server &server)
{
    ParseRequestLineResult parseResult = parseHttpRequestLine(httpRequest, server);
    if (!parseResult.success)
        return (ParseRequestResult::Error(HttpResponse(parseResult.error)));

    ParseHeaderResult headersResult = parseHttpHeaders(httpRequest);
    if (!headersResult.success)
        return (ParseRequestResult::Error(HttpResponse(headersResult.error)));

    Headers headers = headersResult.value;
    ParseBodyResult body = parseHttpBody(httpRequest, headers);
    if (!body.success)
        return (ParseRequestResult::Error(HttpResponse(body.error)));

    RequestLine requestLine = parseResult.value;
    HttpRequest result(requestLine.method, requestLine.uri, requestLine.version, headers, body.value);
    return (ParseRequestResult::Success(result));
}
