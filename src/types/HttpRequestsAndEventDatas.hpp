#pragma once
#include "EventData.hpp"
#include "HttpRequest.hpp"
#include "external.hpp"

struct HttpRequestsAndEventDatas
{
    const std::vector<const HttpRequest> httpRequests;
    const std::vector<const EventData> eventDatas;

    HttpRequestsAndEventDatas() : httpRequests(), eventDatas()
    {
    }
    ~HttpRequestsAndEventDatas()
    {
    }
    HttpRequestsAndEventDatas(const std::vector<const HttpRequest> &httpRequests,
                              const std::vector<const EventData> &eventDatas)
        : httpRequests(httpRequests), eventDatas(eventDatas)
    {
    }
    HttpRequestsAndEventDatas(const HttpRequestsAndEventDatas &other)
        : httpRequests(other.httpRequests), eventDatas(other.eventDatas)
    {
    }
    HttpRequestsAndEventDatas &operator=(const HttpRequestsAndEventDatas &other)
    {
        if (this != &other)
        {
            const_cast<std::vector<const HttpRequest> &>(httpRequests) = other.httpRequests;
            const_cast<std::vector<const EventData> &>(eventDatas) = other.eventDatas;
        }
        return *this;
    }
};
