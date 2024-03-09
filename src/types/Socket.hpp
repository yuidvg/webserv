#pragma once

class Socket
{
  private:
    std::string _receivedMessage;
    std::string _toBeSentMessage;

  public:
    const int descriptor;
    const unsigned int port;
    const std::string opponentIp;
    const unsigned int opponentPort;

    Socket() : descriptor(0), port(0), opponentIp(""), opponentPort(0){};
    Socket(const int descriptor, const unsigned int port, const std::string &opponentIp = "",
           const unsigned int opponentPort = 0)
        : descriptor(descriptor), port(port), opponentIp(opponentIp), opponentPort(opponentPort){};
    ~Socket(){};

    Socket(const Socket &other)
        : _receivedMessage(other._receivedMessage), _toBeSentMessage(other._toBeSentMessage),
          descriptor(other.descriptor), port(other.port), opponentIp(other.opponentIp), opponentPort(other.opponentPort)
    {
    }
    Socket &operator=(const Socket &other)
    {
        if (this != &other)
        {
            _receivedMessage = other._receivedMessage;
            _toBeSentMessage = other._toBeSentMessage;
        }
        return *this;
    }

    bool operator==(const Socket &other) const
    {
        return descriptor == other.descriptor && port == other.port && opponentIp == other.opponentIp &&
               opponentPort == other.opponentPort;
    };
    bool operator!=(const Socket &other) const
    {
        return !(*this == other);
    };

    bool receiveMessage(intptr_t sizeData)
    {
        size_t size = sizeData;
        char *buffer = new char[size + 1];
        const ssize_t receivedLength = recv(descriptor, buffer, size, 0);
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
    void appendToBeSentMessage(const std::string &message)
    {
        _toBeSentMessage += message;
    };
    bool sendMessage(uintptr_t size)
    {
        std::string message = _toBeSentMessage.substr(0, size);
        // TODO:error
        if (send(descriptor, message.c_str(), message.size(), 0) >= 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
};

// TODO:compile errorが発生しているため、一時的に復活
typedef std::vector<Socket> Sockets;
