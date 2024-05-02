#include "../all.hpp"

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
