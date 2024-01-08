#include "RequestParser.hpp"

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
        errorCode = METHOD_NOT_ALLOWED;
        return (false);
    }
    if (version != "Http/1.1")
    {
        errorCode = Http_VERSION_NOT_SUPPORTED;
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
        return (ParseRequestLineResult::Err(BAD_REQUEST));
    if (isLineTooLong(line) == true)
        return (ParseRequestLineResult::Err(REQUEST_URI_TOO_LONG));

    std::istringstream requestLine(line);
    if (!(requestLine >> method >> uri >> version) ||
        !requestLine.eof()) // メソッドとターゲット、バージョンに分けて格納する
        return (ParseRequestLineResult::Err(BAD_REQUEST));

    /* エラーチェック */
    int errorCode = OK;
    if (!checkRequestLine(method, uri, version, errorCode, server))
        return (ParseRequestLineResult::Err(errorCode));

    RequestLine requestLineData = {method, uri, version};
    return (ParseRequestLineResult::Ok(requestLineData));
}

ParseHeaderResult parseHttpHeaders(std::istream &httpRequest)
{
    std::string line;
    std::map<std::string, std::string> header;

    while (std::getline(httpRequest, line))
    {
        if (line.empty())
            break;
        if (isLineTooLong(line) == true)
            return (ParseHeaderResult::Err(REQUEST_URI_TOO_LONG));
        if (std::isspace(line[0]))
            return (ParseHeaderResult::Err(BAD_REQUEST));

        // ヘッダーの解析
        std::string key, value;
        std::istringstream headerLine(line);

        std::getline(headerLine, key, ':');
        std::getline(headerLine, value);
        utils::trim(value); // valueの前後の空白を削除する

        if (key.empty() || std::isspace(*(key.end() - 1)) || value.empty())
            return (ParseHeaderResult::Err(BAD_REQUEST));

        header[utils::toLower(key)] = value; // keyを小文字に変換して格納する
        std::cout << "[key]: " << key << ", [value]: " << header[utils::toLower(key)] << std::endl; // debug
    }
    if (!line.empty() || header.empty())
        return (ParseHeaderResult::Err(BAD_REQUEST));

    return (ParseHeaderResult::Ok(header));
}

ParseBodyResult parseChunkedBody(std::istream &httpRequest, std::map<std::string, std::string> &header)
{
    std::cout << "====parseChunkedBody====" << std::endl; // debug
    std::string line;

    if (header["transfer-encoding"] != "chunked")
        return (ParseBodyResult::Err(NOT_IMPLEMENTED));

    std::string body;
    while (std::getline(httpRequest, line))
    {
        // if (line.empty())
        // 	break ;
        if (isLineTooLong(line) == true)
            return (ParseBodyResult::Err(REQUEST_URI_TOO_LONG));

        std::istringstream chunkSizeLine(line);
        int chunkSize;
        if (!(chunkSizeLine >> std::hex >> chunkSize) || !chunkSizeLine.eof())
            return (ParseBodyResult::Err(BAD_REQUEST));

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

    return (ParseBodyResult::Ok(body));
}

ParseBodyResult parsePlainBody(std::istream &httpRequest, std::map<std::string, std::string> &header)
{
    std::cout << "====parsePlainBody====" << std::endl; // debug
    std::string line;

    if (header["content-length"].empty() || !utils::isNumber(header["content-length"]))
        return (ParseBodyResult::Err(BAD_REQUEST));

    size_t contentLength = std::stoul(header["content-length"]);
    if (contentLength > MAX_LEN)
        return (ParseBodyResult::Err(CONTENT_TOO_LARGE));

    std::cout << "contentLength: " << contentLength << std::endl; // debug
    std::string body(contentLength, '\0');
    if (httpRequest.read(&body[0], contentLength))
        return (ParseBodyResult::Err(BAD_REQUEST));

    return (ParseBodyResult::Ok(body));
}

ParseBodyResult parseHttpBody(std::istream &httpRequest, std::map<std::string, std::string> &header)
{
    std::cout << "====parseBody====" << std::endl; // debug
    std::string line;

    if (header.find("transfer-encoding") != header.end())
        return (parseChunkedBody(httpRequest, header));
    else if (header.find("content-length") != header.end())
        return (parsePlainBody(httpRequest, header));
    else
        return (ParseBodyResult::Ok(""));
}

HttpParseResult parseHttpRequest(std::istream &httpRequest, const Server &server)
{
    ParseRequestLineResult parseResult = parseHttpRequestLine(httpRequest, server);
    if (!parseResult.ok())
        return (HttpParseResult::Err(parseResult.unwrapErr()));

    ParseHeaderResult headers = parseHttpHeaders(httpRequest);
    if (!headers.ok())
        return (HttpParseResult::Err(headers.unwrapErr()));

    std::map<std::string, std::string> header = headers.unwrap();
    ParseBodyResult body = parseHttpBody(httpRequest, header);
    if (!body.ok())
        return (HttpParseResult::Err(body.unwrapErr()));

    RequestLine requestLine = parseResult.unwrap();
    ParsedRequest result(requestLine.method, requestLine.uri, requestLine.version, headers.unwrap(), body.unwrap());
    return (HttpParseResult::Ok(result));
}
