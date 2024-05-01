#include "../all.hpp"

void processHttpMessageFromCgi(const HttpMessage &httpMessage)
{
    if (httpMessage.tag == LEFT)
    {
        httpRequests.push(httpMessage.leftValue);
    }
    else
    {
        const HttpResponse httpResponse = httpMessage.rightValue;
        appendOutbound(httpResponse.destinationSocket, stringify(httpResponse));
    }
}

void processCgiMessage(const ConnectedUnixSocket &socket, const std::string message)
{
    ParseCgiResponseResult parsed = parseCgiResponse(message, socket.descriptor);
    if (parsed.success)
    {
        const CgiResponse &cgiResponse = parsed.value;
        HttpMessage httpMessageFromCgiResponse = processCgiResponse(cgiResponse);
        if (httpMessageFromCgiResponse.tag == LEFT)
        {
            const HttpRequest &httpRequest = httpMessageFromCgiResponse.leftValue;
            httpRequests.push(httpRequest);
        }
        else
        {
            const HttpResponse &httpResponse = httpMessageFromCgiResponse.rightValue;
            appendOutbound(httpResponse.destinationSocket, stringify(httpResponse));
        }
    }
    else
    {
        std::cerr << "Failed to parse CGI response: " << parsed.error << std::endl;
    }
}

std::pair<const HttpResponses, const CgiRequests> processHttpRequests(const HttpRequests &httpRequests)
{
    HttpResponses httpResponses;
    CgiRequests cgiRequests;
    for (HttpRequests::const_iterator it = httpRequests.begin(); it != httpRequests.end(); ++it)
    {
        const HttpRequest &httpRequest = *it;
        const CgiRequestOrHttpResponse cgiRequestOrHttpResponse = processHttpRequest(httpRequest);
        if (cgiRequestOrHttpResponse.tag == RIGHT) // HttpResponse
        {
            httpResponses.push_back(cgiRequestOrHttpResponse.rightValue);
        }
        else // CgiRequest
        {
            cgiRequests.push_back(cgiRequestOrHttpResponse.leftValue);
        }
    }
    return std::make_pair(httpResponses, cgiRequests);
}
