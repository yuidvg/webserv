#include "../all.hpp"

std::pair< HttpResponses, EventDatas > processHttpRequests(const HttpRequests &httpRequests)
{
    HttpResponses httpResponses;
    EventDatas eventDatas;

    for (HttpRequests::const_iterator it = httpRequests.begin(); it != httpRequests.end(); ++it)
    {
        const std::pair< Option< HttpResponse >, Option< EventData > > httpResponse = processHttpRequest(*it);
        if (httpResponse.first)
        {
            httpResponses.push_back(*(httpResponse.first));
        }
        if (httpResponse.second)
        {
            eventDatas.push_back(*(httpResponse.second));
        }
    }
    return std::make_pair(httpResponses, eventDatas);
}
