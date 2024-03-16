#pragma once
#include "../utils/.hpp"
#include "external.hpp"

class Connection
{
  private:
    std::string _receivedMessage;
    std::string _toBeSentMessage;

  public:
    const int sd;
    const unsigned int port;
    const std::string opponentIp;
    const unsigned int opponentPort;

    Connection() : sd(0), port(0), opponentIp(""), opponentPort(0){};
    Connection(const int sd, const unsigned int port, const std::string &opponentIp = "",
               const unsigned int opponentPort = 0)
        : sd(sd), port(port), opponentIp(opponentIp), opponentPort(opponentPort){};
    ~Connection(){};
    Connection(const Connection &other)
        : _receivedMessage(other._receivedMessage), _toBeSentMessage(other._toBeSentMessage), sd(other.sd),
          port(other.port), opponentIp(other.opponentIp), opponentPort(other.opponentPort)
    {
    }
    Connection &operator=(const Connection &other)
    {
        if (this != &other)
        {
            _receivedMessage = other._receivedMessage;
            _toBeSentMessage = other._toBeSentMessage;
        }
        return *this;
    }

    bool operator==(const Connection &other) const
    {
        return sd == other.sd && port == other.port && opponentIp == other.opponentIp &&
               opponentPort == other.opponentPort;
    };
    bool operator!=(const Connection &other) const
    {
        return !(*this == other);
    };

    bool receiveMessage(intptr_t sizeData)
    {
        size_t size = sizeData;
        char *buffer = new char[size + 1];
        const ssize_t receivedLength = recv(sd, buffer, size, 0);
        if (receivedLength >= 0)
        {
            buffer[receivedLength] = '\0';
            _receivedMessage += buffer;
        }
        delete[] buffer;
        return receivedLength >= 0;
    };
    std::string getReceivedMessage() const
    {
        return _receivedMessage;
    };
    bool appendToBeSentMessage(const std::string &message)
    {
        _toBeSentMessage += message;
        if (!utils::setEventFlags(sd, EVFILT_WRITE, EV_ENABLE))
        {
            std::cerr << "failed to set event flags" << std::endl;
            return false;
        }
        return true;
    };
    bool hasMessageToSend() const
    {
        return !_toBeSentMessage.empty();
    };
    bool sendMessage(uintptr_t size)
    {
        std::string message = _toBeSentMessage.substr(0, size);
        const int sentLength = send(sd, message.c_str(), message.size(), 0);
        if (sentLength >= 0)
        {
            std::cout << "message sent: " << sentLength << std::endl;
            _toBeSentMessage = _toBeSentMessage.substr(sentLength);
            if (_toBeSentMessage.empty())
                if (!utils::setEventFlags(sd, EVFILT_WRITE, EV_DISABLE))
                    return false;
            return true;
        }
        else
            return false;
    };
    bool isListenSocket() const
    {
        return opponentIp.empty() && opponentPort == 0;
    };
};

// TODO:compile errorが発生しているため、一時的に復活
