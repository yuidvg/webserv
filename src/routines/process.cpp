#include "../all.hpp"

void processHttpMessageFromCgi(const HttpMessage &httpMessage)
{
    if (httpMessage.tag == LEFT)
    {
        HTTP_REQUESTS.push(httpMessage.leftValue);
    }
    else
    {
        const HttpResponse httpResponse = httpMessage.rightValue;
        if (!appendOutbound(httpResponse.destinationSd, stringify(httpResponse)))
        {
            std::cerr << "Failed to set outbound for socket " << httpResponse.destinationSd << std::endl;
        }
    }
}

void processClientMessages(const ConnectedInternetSocket &socket, SocketBuffer &socketBuffer)
{
    // messageをパースして、httpRequestsに変換
    ParsedHttpRequests parsed = parseHttpRequests(socketBuffer, socket);
    socketBuffer.substringInbound(parsed.size);
    utils::appendQueue(parsed.httpRequests, HTTP_REQUESTS);
}

void processCgiMessage(const ConnectedUnixSocket &socket, const std::string message)
{
    ParseCgiResponseResult parsed = parseCgiResponse(message, socket.descriptor);
    if (parsed.success)
    {
        const CgiResponse &cgiResponse = parsed.value;
        processCgiResponse(cgiResponse);
    }
    else
    {
        std::cerr << "Failed to parse CGI response: " << parsed.error << std::endl;
    }
}
