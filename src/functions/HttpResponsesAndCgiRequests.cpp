#include "../all.hpp"

HttpResponses_CgiRequests_EventDatas processHttpRequests(const HttpRequests &httpRequests)
{
    HttpResponses httpResponses;
    CgiRequests cgiRequests;
    EventDatas eventDatas;
    for (HttpRequests::const_iterator it = httpRequests.begin(); it != httpRequests.end(); ++it)
    {
        const HttpRequest &httpRequest = *it;
        const HttpResponseOrCgiRequestOrEventData httpResponseOrCgiRequestOrEventData = processHttpRequest(httpRequest);
        if (httpResponseOrCgiRequestOrEventData.tag == TERNARY_FIRST) // HttpResponse
        {
            httpResponses.push_back(httpResponseOrCgiRequestOrEventData.first);
        }
        else if (httpResponseOrCgiRequestOrEventData.tag == TERNARY_SECOND) // CgiRequest
        {
            cgiRequests.push_back(httpResponseOrCgiRequestOrEventData.second);
        }
        else
        {
            eventDatas.push_back(httpResponseOrCgiRequestOrEventData.third);
        }
    }
    return std::make_pair(httpResponses, std::make_pair(cgiRequests, eventDatas));
}
