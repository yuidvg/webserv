#pragma once
#include "Socket.hpp"
#include "external.hpp"

class Sockets
{
  private:
    std::vector<Socket> _sockets;

  public:
    Sockets() : _sockets(){};
    ~Sockets(){};
    Socket &operator[](const int descriptor)
    {
        for (std::vector<Socket>::iterator it = _sockets.begin(); it != _sockets.end(); ++it)
        {
            if (it->descriptor == descriptor)
            {
                return *it;
            }
        }
        return *_sockets.end();
    };
    void operator+=(const Socket &socket)
    {
        _sockets.push_back(socket);
    };
    void operator+=(const Sockets &sockets)
    {
        for (std::vector<Socket>::const_iterator it = sockets._sockets.begin(); it != sockets._sockets.end(); ++it)
        {
            _sockets.push_back(*it);
        }
    };
    void operator-=(const Socket &socket)
    {
        for (std::vector<Socket>::iterator it = _sockets.begin(); it != _sockets.end(); ++it)
        {
            if (*it == socket)
            {
                _sockets.erase(it);
                return;
            }
        }
    };
};
