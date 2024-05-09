#include "../all.hpp"

static bool isLineTooLong(const std::string &line)
{
    if (line.length() > MAX_LEN)
        return true;
    else
        return false;
}

/* Parse RequestLine */

bool validateAndDecodeTarget(std::string &target)
{
    std::string decodedTarget;
    std::string::iterator it = target.begin();

    while (it != target.end())
    {
        if (*it == '%')
        {
            if (it + 2 >= target.end() || !std::isxdigit(*(it + 1)) || !std::isxdigit(*(it + 2)))
                return false;
            decodedTarget += utils::hexToUtf8Char(target.substr(it - target.begin() + 1, 2));
            it += 3;
        }
        else
        {
            decodedTarget += *it;
            it++;
        }
    }
    target = decodedTarget;
    return true;
}

static SplitRequestLineResult splitRequestLine(std::string &requestLine)
{
    std::string method, target, version;
    std::istringstream requestLineStream(requestLine);

    if (!requestLineStream.eof() && !isLineTooLong(requestLine))
    {
        if ((requestLineStream >> method >> target >> version) && requestLineStream.eof() &&
            validateAndDecodeTarget(target))
            return SplitRequestLineResult::Success(RequestLine(method, target, version));
        else
            return SplitRequestLineResult::Error("Invalid Request Line");
    }
    else
        return SplitRequestLineResult::Error("Invalid Request Line");
}

static bool isValidRequestLine(const RequestLine requestLine)
{
    if (requestLine.target.find(':') != std::string::npos &&
        requestLine.target.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
        return false;

    if (requestLine.version != SERVER_PROTOCOL)
        return false;

    return true;
}

static ParseRequestLineResult parseHttpRequestLine(std::string &firstLine)
{
    const SplitRequestLineResult splitRequestLineResult = splitRequestLine(firstLine);
    if (splitRequestLineResult.success)
    {
        if (isValidRequestLine(splitRequestLineResult.value))
            return ParseRequestLineResult::Success(splitRequestLineResult.value);
        else
            return ParseRequestLineResult::Error(splitRequestLineResult.error);
    }
    else
        return ParseRequestLineResult::Error(splitRequestLineResult.error);
}

/*.Parse Header */
// 　のちのち、option型にする
static ParseHeaderResult parseHeader(std::istringstream &requestTextStream)
{
    // tellgで現在の位置を取得、その先からCRLFまでstringを取得する
    const std::streampos currentPos = requestTextStream.tellg();
    const std::string requestStr = requestTextStream.str();
    const std::size_t headerEndPos = requestStr.find("\r\n\r\n", currentPos);
    const std::streampos endPos = (headerEndPos != std::string::npos) ? headerEndPos + 4 : requestStr.length();
    const std::string headerText = requestStr.substr(currentPos, endPos - currentPos);

    const std::vector<std::string> header = utils::split(headerText, CRLF);
    if (header.size() == 0)
        return ParseHeaderResult::Pending();
    else
    {
        Headers headers;
        for (std::vector<std::string>::const_iterator it = header.begin(); it != header.end(); ++it)
        {
            const std::string::size_type pos = it->find(": ");
            if (pos != std::string::npos)
            {
                std::string key = it->substr(0, pos);
                std::string value = it->substr(pos + 1);
                std::transform(key.begin(), key.end(), key.begin(), ::tolower); // headerのkeyは大文字小文字を区別しない
                value = utils::trim(value);                                     // valueの前後の空白を削除
                headers[key] = value;
            }
            else
                return ParseHeaderResult::Success(Headers());
        }
        return ParseHeaderResult::Success(headers);
    }
}

/* Parse FirstBlock */

static ParseFirstBlockResult parseFirstBlock(const std::string &block)
{
    // blockの後ろがCRLFの場合は、まだ読み込みが終わっていないのでPENDINGを返す
    if (block.substr(block.length() - 2) == CRLF)
        return ParseFirstBlockResult::Pending();
    else
    {
        std::istringstream requestTextStream(block);
        // 最初に空行がある場合は読み飛ばす
        std::string firstLine = utils::getlineCustom(requestTextStream);
        while (firstLine.empty() && !requestTextStream.eof())
            firstLine = utils::getlineCustom(requestTextStream);

        const ParseRequestLineResult parseRequestLineResult = parseHttpRequestLine(firstLine);
        if (parseRequestLineResult.success)
        {
            const ParseHeaderResult parseHeaderResult = parseHeader(requestTextStream);
            if (parseHeaderResult.status == PARSED)
            {
                if (parseHeaderResult.value.find("host") != parseHeaderResult.value.end())
                    return ParseFirstBlockResult::Success(
                        FirstBlock(parseRequestLineResult.value, parseHeaderResult.value));
                else
                    return ParseFirstBlockResult::Error(block);
            }
            else if (parseHeaderResult.status == PENDING)
                return ParseFirstBlockResult::Pending();
        }
        return ParseFirstBlockResult::Error(block);
    }
}

/* Parse Body */

static UnchunkBodyResult unchunkBody(const std::string &body, const size_t maxBodySize)
{
    if (body.find("\r\n0") == std::string::npos)
    {
        return UnchunkBodyResult::Pending();
    }
    else
    {
        const std::vector<std::string> chunks = utils::split(body, CRLF);
        unsigned long chunkSize;
        std::string unchunkedBody = "";
        for (size_t i = 0; i < chunks.size(); i++)
        {
            if (i % 2 == 0)
            {
                if (!utils::isHex(chunks[i]))
                    return UnchunkBodyResult::Error("Invalid Body");
                chunkSize = std::strtoul(chunks[i].c_str(), NULL, 16);
                if (chunkSize == 0)
                    return UnchunkBodyResult::Success(unchunkedBody);
            }
            else
            {
                if (chunks[i].length() <= maxBodySize)
                {
                    if (chunks[i].length() == chunkSize)
                    {
                        if (i == chunks.size() - 1 || unchunkedBody.length() + chunkSize > maxBodySize)
                        {
                            return UnchunkBodyResult::Error("Invalid Body");
                        }
                        else
                        {
                            unchunkedBody += chunks[i];
                        }
                    }
                    else
                        return UnchunkBodyResult::Error("Invalid Body");
                }
                else
                    return UnchunkBodyResult::Error("Invalid Body");
            }
        }
        return UnchunkBodyResult::Error("Invalid Body");
    }
}

static ParseBodyResult parseBody(const std::string &bodyBlock, const Headers &headers, const size_t maxBodySize)
{
    const std::string body = utils::split(bodyBlock, CRLF + CRLF)[0];
    if (headers.find("transfer-encoding") != headers.end())
    {
        if (headers.at("transfer-encoding") == "chunked" && body.length() <= maxBodySize)
        {
            const UnchunkBodyResult unchunkedBodyResult = unchunkBody(body, maxBodySize);
            if (unchunkedBodyResult.status == PARSED)
                return ParseBodyResult::Success(unchunkedBodyResult.value);
            else if (unchunkedBodyResult.status == PENDING)
                return ParseBodyResult::Pending();
            else
                return ParseBodyResult::Error(unchunkedBodyResult.error);
        }
        else
            return ParseBodyResult::Error("Invalid body");
    }
    else if (headers.find("content-length") != headers.end())
    {
        if (utils::isNumber(headers.at("content-length")))
        {
            const size_t bodySize = std::strtoul(headers.at("content-length").c_str(), NULL, 10);
            if ((0 <= bodySize && bodySize <= maxBodySize) && body.length() == bodySize)
                return ParseBodyResult::Success(body);
            else
                return ParseBodyResult::Error("Invalid body");
        }
        else
            return ParseBodyResult::Error("Invalid body");
    }
    else
        return ParseBodyResult::Error("Invalid header");
}

static std::string getHostName(const Headers &headers)
{
    if (headers.find("host") != headers.end())
        return headers.at("host");
    else
        return "";
}

static EventDataOrParsedRequest parseHttpRequest(const Socket &socket, const std::string &request)
{
    const std::vector<std::string> blocks = utils::split(request, CRLF + CRLF);
    const ParseFirstBlockResult parseFirstBlockResult = parseFirstBlock(blocks[0]);
    if (parseFirstBlockResult.status == PARSED)
    {
        const RequestLine requestLine = parseFirstBlockResult.value.requestLine;
        const std::string host = getHostName(parseFirstBlockResult.value.headers);
        if (!host.empty() && blocks.size() == 2 && blocks[0].find("POST") != std::string::npos)
        {
            const ParseBodyResult parseBodyResult =
                parseBody(blocks[1], parseFirstBlockResult.value.headers,
                          CONFIG.getServer(host, socket.serverPort).clientMaxBodySize);
            if (parseBodyResult.status == PARSED)
                return EventDataOrParsedRequest::Right(HttpRequest(socket, host, requestLine.method, requestLine.target,
                                                                   parseFirstBlockResult.value.headers,
                                                                   parseBodyResult.value));
            else if (parseBodyResult.status == PENDING)
                return EventDataOrParsedRequest::Left(EventData(socket, request));
            else
                return EventDataOrParsedRequest::Right(HttpRequest(socket));
        }
        else
            return EventDataOrParsedRequest::Right(HttpRequest(socket, host, requestLine.method, requestLine.target,
                                                               parseFirstBlockResult.value.headers, ""));
    }
    else if (parseFirstBlockResult.status == PENDING)
        return EventDataOrParsedRequest::Left(EventData(socket, request));
    else
        return EventDataOrParsedRequest::Right(HttpRequest(socket));
}

static bool isBody(const std::string &block)
{
    return block.find("POST") == std::string::npos && block.find("GET") == std::string::npos &&
           block.find("DELETE") == std::string::npos;
}

static std::vector<std::string> splitHttpRequests(const EventData &eventData)
{
    std::vector<std::string> httpRequests;
    const std::vector<std::string> blocks = utils::split(eventData.data, CRLF + CRLF);
    for (size_t i = 0; i < blocks.size(); i++)
    {
        if (blocks[i].find("POST") != std::string::npos && i != blocks.size() - 1 && isBody(blocks[i + 1]))
        {
            const std::string postBlock = blocks[i] + blocks[i + 1];
            httpRequests.push_back(postBlock);
            i++;
        }
        else
            httpRequests.push_back(blocks[i]);
    }
    return httpRequests;
}

std::pair<HttpRequests, EventDatas> parseHttpRequests(const EventDatas &httpRequestEventDatas)
{
    HttpRequests httpRequests;
    EventDatas eventDatas;
    for (size_t i = 0; i < httpRequestEventDatas.size(); i++)
    {
        const std::vector<std::string> requests = splitHttpRequests(httpRequestEventDatas[i]);
        for (size_t j = 0; j < requests.size(); j++)
        {
            const EventDataOrParsedRequest eventDataOrParsedRequest =
                parseHttpRequest(httpRequestEventDatas[i].socket, requests[j]);
            if (eventDataOrParsedRequest.tag == LEFT)
                eventDatas.push_back(eventDataOrParsedRequest.leftValue);
            else
                httpRequests.push_back(eventDataOrParsedRequest.rightValue);
        }
    }
    return std::make_pair(httpRequests, eventDatas);
}