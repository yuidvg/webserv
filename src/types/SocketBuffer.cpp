#include "../all.hpp"

SocketBuffer::SocketBuffer() : _outbound(""), _inbound(""), sd(-1)
{
}

SocketBuffer::~SocketBuffer()
{
}

SocketBuffer::SocketBuffer(const SocketBuffer &other)
    : _outbound(other._outbound), _inbound(other._inbound), sd(other.sd)
{
}

SocketBuffer &SocketBuffer::operator=(const SocketBuffer &other)
{
    if (this != &other)
    {
        _outbound = other._outbound;
        _inbound = other._inbound;
        sd = other.sd;
    }
    return *this;
}

SocketBuffer::SocketBuffer(const int sd) : _outbound(""), _inbound(""), sd(sd)
{
}

bool SocketBuffer::operator==(const SocketBuffer &other) const
{
    return sd == other.sd;
}

ReceiveResult SocketBuffer::receiveInbound(size_t size)
{
    if (size > 0)
    {
        try
        {
            char *buffer = new char[size + 1]; // +1 for null terminator
            const ssize_t receivedLength = recv(sd, buffer, size, 0);
            if (receivedLength < 0)
            {
                delete[] buffer;
                return ReceiveResult::Error("Failed to receive message");
            }
            if (receivedLength == 0)
            {
                delete[] buffer;
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
    return ReceiveResult::Success(0);
}

std::string SocketBuffer::getInbound() const
{
    return _inbound;
}

void SocketBuffer::substringInbound(size_t size)
{
    _inbound = _inbound.substr(size);
}

void SocketBuffer::appendOutbound(std::string message)
{
    if (_outbound.empty() && message.size())
        utils::setEventFlags(sd, EVFILT_WRITE, EV_ENABLE);
    _outbound += message;
}

SendResult SocketBuffer::sendOutbound(size_t size)
{
    const ssize_t sentLength = _outbound.size() ? write(sd, _outbound.c_str(), std::min(size, _outbound.size())) : 0;
    if (sentLength < 0)
    {
        return SendResult::Error("Failed to send message");
    }
    _outbound = _outbound.substr(sentLength);
    if (_outbound.empty())
        utils::setEventFlags(sd, EVFILT_WRITE, EV_DISABLE);
    return SendResult::Success(sentLength);
}
