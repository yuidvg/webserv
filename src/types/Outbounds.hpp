#pragma once
#include "Event.hpp"
#include "EventData.hpp"
typedef std::vector< EventData > EventDatas;
typedef std::vector< const Event > Events;

class Outbounds
{
  private:
    EventDatas outbounds;
    EventDatas::iterator outboundWithSameSocket(const EventData &eventData);
    EventDatas::iterator findOutbound(const Socket &socket);

  public:
    Outbounds();
    ~Outbounds();
    void push_back(const EventData &eventData);
    void append_back(const EventDatas &eventDatas);
    void dispatchEvents(const Events &events);
};
