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

    SocketBuffer();
    ~SocketBuffer();
    SocketBuffer(const SocketBuffer &other);
    SocketBuffer &operator=(const SocketBuffer &other);
    SocketBuffer(const int sd);
    bool operator==(const SocketBuffer &other) const;

    ReceiveResult receiveInbound(size_t size);
    std::string getInbound() const;
    void substringInbound(size_t size);
    void appendOutbound(std::string message);
    SendResult sendOutbound(size_t size);
};