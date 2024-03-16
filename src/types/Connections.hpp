#pragma once
#include "Connection.hpp"
#include "external.hpp"

class Connections
{
  private:
    std::vector<Connection> _connections;

  public:
    Connections() : _connections(){};
    ~Connections(){};
    Connection &operator[](const int sd)
    {
        for (std::vector<Connection>::iterator it = _connections.begin(); it != _connections.end(); ++it)
        {
            if (it->sd == sd)
            {
                return *it;
            }
        }
        return *_connections.end();
    };
    void operator+=(const Connection &socket)
    {
        _connections.push_back(socket);
    };
    void operator+=(const Connections &sockets)
    {
        for (std::vector<Connection>::const_iterator it = sockets._connections.begin(); it != sockets._connections.end(); ++it)
        {
            _connections.push_back(*it);
        }
    };
    void operator-=(const Connection &socket)
    {
        for (std::vector<Connection>::iterator it = _connections.begin(); it != _connections.end(); ++it)
        {
            if (*it == socket)
            {
                _connections.erase(it);
                return;
            }
        }
    };
};
