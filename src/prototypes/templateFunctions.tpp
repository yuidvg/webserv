#pragma once
#include "top.hpp"

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
