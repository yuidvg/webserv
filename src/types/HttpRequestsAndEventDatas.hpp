#pragma once
#include "EventData.hpp"
#include "HttpRequest.hpp"
#include "external.hpp"

struct HttpRequestsAndEventDatas
{
    const std::queue<const HttpRequest> httpRequests;
    const std::queue<const EventData> eventDatas;

    HttpRequestsAndEventDatas() : httpRequests(), eventDatas()
    {
    }
    ~HttpRequestsAndEventDatas()
    {
    }
    HttpRequestsAndEventDatas(const std::queue<const HttpRequest> &httpRequests,
                              const std::queue<const EventData> &eventDatas)
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
            const_cast<std::queue<const HttpRequest> &>(httpRequests) = other.httpRequests;
            const_cast<std::queue<const EventData> &>(eventDatas) = other.eventDatas;
        }
        return *this;
    }
};
