#pragma once
#include "Result.hpp"
#include "external.hpp"

#define BUFFER_SIZE 1024
typedef Result<const ssize_t, const std::string> ReceiveResult;
typedef Result<const ssize_t, const std::string> SendResult;

class SocketIO
{
  private:
    std::string _outbound;
    std::string _inbound;

  public:
    int sd;

    SocketIO();
    ~SocketIO();
    SocketIO(const SocketIO &other);
    SocketIO &operator=(const SocketIO &other);

    SocketIO(const int sd);
    bool operator==(const SocketIO &other) const;


    ReceiveResult receiveInbound(size_t);
    std::string getInbound();
    void clearInbound();
    void setOutbound(std::string);
    SendResult sendOutbound();
};

SocketIO::SocketIO() : _outbound(""), _inbound(""), sd(-1)
{
}

SocketIO::~SocketIO()
{
}

SocketIO::SocketIO(const SocketIO &other) : _outbound(other._outbound), _inbound(other._inbound), sd(other.sd)
{
}

SocketIO &SocketIO::operator=(const SocketIO &other)
{
    if (this != &other)
    {
        _outbound = other._outbound;
        _inbound = other._inbound;
        sd = other.sd;
    }
    return *this;
}

SocketIO::SocketIO(const int sd) : _outbound(""), _inbound(""), sd(sd)
{
}

bool SocketIO::operator==(const SocketIO &other) const
{
    return sd == other.sd;
}

ReceiveResult SocketIO::receiveInbound(size_t size)
{
    try
    {
        char *buffer = new char[size];
        const ssize_t receivedLength = recv(sd, buffer, sizeof(buffer), 0);
        if (receivedLength < 0)
        {
            return ReceiveResult::Error("Failed to receive message");
        }
        if (receivedLength == 0)
        {
            return ReceiveResult::Error("Connection closed");
        }
        buffer[receivedLength] = '\0';
        _inbound += buffer;
        delete[] buffer;
        return ReceiveResult::Success(receivedLength);
    }
    catch (std::bad_alloc &e)
    {
        return ReceiveResult::Error(e.what());
    }
}

std::string SocketIO::getInbound()
{
    return _inbound;
}

void SocketIO::clearInbound()
{
    _inbound = "";
}

void SocketIO::setOutbound(std::string message)
{
    _outbound = message;
}

SendResult SocketIO::sendOutbound()
{
    const ssize_t sentLength = send(sd, _outbound.c_str(), _outbound.size(), 0);
    if (sentLength < 0)
    {
        return SendResult::Error("Failed to send message");
    }
    _outbound = _outbound.substr(sentLength);
    return SendResult::Success(sentLength);
}
