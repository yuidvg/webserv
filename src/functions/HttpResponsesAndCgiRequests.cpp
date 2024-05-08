#include "../all.hpp"

HttpResponses processHttpRequests(const HttpRequests &httpRequests)
{
    HttpResponses httpResponses;

    for (HttpRequests::const_iterator it = httpRequests.begin(); it != httpRequests.end(); ++it)
    {
        const Option<HttpResponse> httpResponse = processHttpRequest(*it);
        if (httpResponse)
        {
            httpResponses.push_back(*httpResponse);
        }
    }
    return httpResponses;
}
