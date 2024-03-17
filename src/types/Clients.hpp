#pragma once
#include "Client.hpp"
#include "external.hpp"

class Clients
{
  private:
    std::vector<Client> _clients;

  public:
    Clients() : _clients(){};
    ~Clients(){};
    Client &operator[](const int sd)
    {
        for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        {
            if (it->sd == sd)
            {
                return *it;
            }
        }
        return *_clients.begin();
    };
    bool has(const int sd) const
    {
        for (std::vector<Client>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
        {
            if (it->sd == sd)
            {
                return true;
            }
        }
        return false;
    };
    void operator+=(const Client &client)
    {
        _clients.push_back(client);
    };
    void operator+=(const Clients &clients)
    {
        for (std::vector<Client>::const_iterator it = clients._clients.begin(); it != clients._clients.end(); ++it)
        {
            _clients.push_back(*it);
        }
    };
    void operator-=(const Client &client)
    {
        for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it)
        {
            if (*it == client)
            {
                _clients.erase(it);
                return;
            }
        }
    };
};
