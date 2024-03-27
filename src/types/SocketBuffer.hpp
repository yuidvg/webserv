#pragma once
#include "Result.hpp"
#include "external.hpp"

#define BUFFER_SIZE 1024
typedef Result<const ssize_t, const std::string> ReceiveResult;
typedef Result<const ssize_t, const std::string> SendResult;

class SocketBuffer
{
  private:
    std::string _outbound;
    std::string _inbound;

  public:
    int sd;

    SocketBuffer() : _outbound(""), _inbound(""), sd(-1){};
    ~SocketBuffer(){};
    SocketBuffer(const SocketBuffer &other) : _outbound(other._outbound), _inbound(other._inbound), sd(other.sd){};
    SocketBuffer &operator=(const SocketBuffer &other)
    {
        if (this != &other)
        {
            _outbound = other._outbound;
            _inbound = other._inbound;
            sd = other.sd;
        }
        return *this;
    };

    SocketBuffer(const int sd) : _outbound(""), _inbound(""), sd(sd){};
    bool operator==(const SocketBuffer &other) const
    {
        return sd == other.sd;
    };

    ReceiveResult receiveInbound(size_t size)
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
    };
    std::string getInbound()
    {
        return _inbound;
    }
    void substringInbound(size_t size)
    {
        _inbound = _inbound.substr(size);
    }
    void appendOutbound(std::string message)
    {
        _outbound += message;
    }
    SendResult sendOutbound(size_t size)
    {
        const ssize_t sentLength = send(sd, _outbound.c_str(), size, 0);
        if (sentLength < 0)
        {
            return SendResult::Error("Failed to send message");
        }
        _outbound = _outbound.substr(sentLength);
        return SendResult::Success(sentLength);
    }
};
