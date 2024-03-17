#pragma once
#include "external.hpp"

namespace utils
{
bool setEventFlags(const uintptr_t identifier, const int16_t filter, const uint16_t flags);

} // namespace utils

class Client
{
  private:
    std::string _receivedMessage;
    std::string _toBeSentMessage;

  public:
    const int sd;
    const unsigned int serverPort;
    const std::string opponentIp;
    const unsigned int opponentPort;

    Client() : _receivedMessage(""), _toBeSentMessage(""), sd(0), serverPort(0), opponentIp(""), opponentPort(0){};
    Client(const int sd, const unsigned int serverPort, const std::string &opponentIp = "",
           const unsigned int opponentPort = 0)
        : _receivedMessage(""), _toBeSentMessage(""), sd(sd), serverPort(serverPort), opponentIp(opponentIp),
          opponentPort(opponentPort){};
    ~Client(){};
    Client(const Client &other)
        : _receivedMessage(other._receivedMessage), _toBeSentMessage(other._toBeSentMessage), sd(other.sd),
          serverPort(other.serverPort), opponentIp(other.opponentIp), opponentPort(other.opponentPort)
    {
    }
    Client &operator=(const Client &other)
    {
        if (this != &other)
        {
            _receivedMessage = other._receivedMessage;
            _toBeSentMessage = other._toBeSentMessage;
        }
        return *this;
    }

    bool operator==(const Client &other) const
    {
        return sd == other.sd && serverPort == other.serverPort && opponentIp == other.opponentIp &&
               opponentPort == other.opponentPort;
    };
    bool operator!=(const Client &other) const
    {
        return !(*this == other);
    };

    void clearReceivedMessage()
    {
        _receivedMessage = "";
    };
    std::string getReceivedMessage() const
    {
        return _receivedMessage;
    };
    bool receiveMessage(size_t size)
    {
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
    void appendToBeSentMessage(const std::string &message)
    {
        _toBeSentMessage += message;
        if (!utils::setEventFlags(sd, EVFILT_WRITE, EV_ENABLE))
            std::cerr << "failed to set event flags" << std::endl;
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
};
