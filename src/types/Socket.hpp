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
        : descriptor(other.descriptor), port(other.port), opponentIp(other.opponentIp), opponentPort(other.opponentPort)
    {
    }
    Socket &operator=(const Socket &other)
    {
        (void)other;
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

    bool receiveMessage(uintptr_t size)
    {
        char *buffer = new char[size];
        const ssize_t receivedLength = recv(descriptor, buffer, size, 0);
        if (receivedLength >= 0)
            _receivedMessage += buffer;
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
    };
    bool sendMessage(uintptr_t size)
    {
        std::string message = _toBeSentMessage.substr(0, size);
        _toBeSentMessage = _toBeSentMessage.substr(size);
        if (send(descriptor, message.c_str(), size, 0) >= 0)
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
