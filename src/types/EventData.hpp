#pragma once
#include "CgiRequest.hpp"
#include "HttpResponse.hpp"
#include "external.hpp"

std::string stringify(const HttpResponse &);

struct EventData
{
    Socket socket;
    std::string data;

    EventData() : socket(), data("")
    {
    }
    ~EventData()
    {
    }
    EventData(const HttpResponse &httpResponse) : socket(httpResponse.destinationSocket), data(stringify(httpResponse))
    {
    }
    EventData(const CgiRequest &cgiRequest) : socket(cgiRequest.cgiSocket), data(cgiRequest.body)
    {
    }
    EventData(const Socket &socket, const std::string &data) : socket(socket), data(data)
    {
    }
    EventData(const EventData &other) : socket(other.socket), data(other.data)
    {
    }
    EventData &operator=(const EventData &other)
    {
        if (this != &other)
        {
            socket = other.socket;
            data = other.data;
        }
        return *this;
    }
    bool operator==(const EventData &other) const
    {
        return socket == other.socket && data == other.data;
    }
    bool operator!=(const EventData &other) const
    {
        return !(*this == other);
    }
};
