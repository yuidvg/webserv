#pragma once
#include "Client.hpp"
#include "HttpRequest.hpp"
#include "external.hpp"

class Cgi
{
  private:
    Client *_client;
    std::string _receivedMessage;
    std::string _toBeSentMessage;

  public:
    const int sd;
    const HttpRequest httpRequest;

    Cgi() : _client(0), _receivedMessage(""), _toBeSentMessage(""), sd(0), httpRequest(HttpRequest()){};
    ~Cgi(){};
    Cgi(const int sd, const HttpRequest httpRequest, Client &_client)
        : _client(&_client), _receivedMessage(""), _toBeSentMessage(""), sd(sd), httpRequest(httpRequest){};
    Cgi(const Cgi &other)
        : _client(other._client), _receivedMessage(other._receivedMessage), _toBeSentMessage(other._toBeSentMessage),
          sd(other.sd), httpRequest(other.httpRequest){};
    Cgi &operator=(const Cgi &other)
    {
        if (this != &other)
        {
            _receivedMessage = other._receivedMessage;
            _toBeSentMessage = other._toBeSentMessage;
        }
        return *this;
    }
    bool operator==(const Cgi &other) const
    {
        return sd == other.sd;
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
    }

    bool sendMessage(size_t size)
    {
        const std::string message = _toBeSentMessage.substr(0, size);
        const ssize_t sentLength = send(sd, message.c_str(), message.size(), 0);
        if (sentLength >= 0)
        {
            _toBeSentMessage = _toBeSentMessage.substr(sentLength);
            return true;
        }
        else
        {
            return false;
        }
    }
    void appendToReceivedMessage(const std::string &message)
    {
        _receivedMessage += message;
    }
    std::string getReceivedMessage() const
    {
        return _receivedMessage;
    }

    void appendToBeSentMessage(const std::string &message)
    {
        _toBeSentMessage += message;
    }
    Client *getClient() const
    {
        return _client;
    }
};
