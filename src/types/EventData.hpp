#pragma once
#include "CgiRequest.hpp"
#include "HttpResponse.hpp"
#include "Option.hpp"
#include "external.hpp"

std::string stringify(const HttpResponse &);

struct EventData
{
    Socket socket;
    std::string data;
    HttpRequest httpRequest;

    EventData() : socket(), data("")
    {
    }
    ~EventData()
    {
    }
    EventData(const HttpResponse &httpResponse)
        : socket(httpResponse.destinationSocket), data(stringify(httpResponse)), httpRequest()
    {
    }
    EventData(const CgiRequest &cgiRequest)
        : socket(cgiRequest.cgiSocket), data(cgiRequest.body), httpRequest(cgiRequest.httpRequest)
    {
    }
    EventData(const Socket &socket, const std::string &data, const HttpRequest &httpRequest = HttpRequest())
        : socket(socket), data(data), httpRequest(httpRequest)
    {
    }
    EventData(const EventData &other) : socket(other.socket), data(other.data), httpRequest(other.httpRequest)
    {
    }
    EventData &operator=(const EventData &other)
    {
        if (this != &other)
        {
            socket = other.socket;
            data = other.data;
            httpRequest = other.httpRequest;
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
