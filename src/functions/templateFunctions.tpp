#pragma once
#include "../types/EventData.hpp"
#include "../types/external.hpp"

typedef std::vector<const EventData> EventDatas;

template <typename T> EventDatas toEventDatas(const std::vector<T> &ts)
{
    EventDatas eventDatas;
    for (typename std::vector<T>::const_iterator it = ts.begin(); it != ts.end(); ++it)
    {
        const T &t = *it;
        eventDatas.push_back(EventData(t));
    }
    return eventDatas;
}
