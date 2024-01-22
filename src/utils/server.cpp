#include "../socket/all.hpp"
#include "all.hpp"

namespace utils
{

namespace
{

Servers filter(const Servers servers, const unsigned int port)
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

Servers filter(const Servers servers, const std::string uri)
{
    Servers filtered;
    for (unsigned int i = 0; i < servers.size(); i++)
    {
        if (servers[i].name == host(uri))
        {
            filtered.push_back(servers[i]);
        }
    }
    return filtered;
}
} // namespace

MatchedServerResult matchedServer(const std::string uri, const Servers servers, const Sd sd)
{
    const PortNumberResult portNumberResult = portNumber(sd);
    if (portNumberResult.success)
    {
        const unsigned int port = portNumberResult.value;
        const Servers serversWithPort = filter(servers, port);
        const Servers serversWithPortAndName = filter(serversWithPort, uri);
        if (serversWithPortAndName.size() > 0)
        {
            return MatchedServerResult::Success(serversWithPortAndName[0]);
        }
        else
        {
            return MatchedServerResult::Error(SERVER_ERROR_RESPONSE);
        }
    }
    else
    {
        return MatchedServerResult::Error(portNumberResult.error);
    }
}
} // namespace utils
