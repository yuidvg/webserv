#pragma once
#include "Server.hpp"

namespace
{
Servers filter(const Servers &servers, const unsigned int &port)
{
    Servers filtered;

    for (unsigned int i = 0; i < servers.size(); i++)
    {
        if (servers[i].port == port)
        {
            filtered.push_back(servers[i]);
        }
    }
    return filtered;
}

Servers filter(const Servers &servers, const std::string &host)
{
    Servers filtered;
    for (unsigned int i = 0; i < servers.size(); i++)
    {
        if (servers[i].name == host)
        {
            filtered.push_back(servers[i]);
        }
    }
    return filtered;
}
} // namespace

class Config
{
  private:
    Servers servers;

  public:
    Config() : servers(){};
    Server getServer(const std::string &host, const unsigned int &port) const
    {
        const Servers serversWithPort = filter(servers, port);
        if (serversWithPort.size() > 0)
        {
            const Servers serversWithPortAndName = filter(serversWithPort, host);
            if (serversWithPortAndName.size() > 0)
            {
                return serversWithPortAndName[0];
            }
            else
            {
                return serversWithPort[0];
            }
        }
        else
        {
            return servers[0];
        }
    }
    void injectServers(const Servers &servers)
    {
        for (Servers::const_iterator it = servers.begin(); it != servers.end(); ++it)
        {
            this->servers.push_back(*it);
        }
    };
};
