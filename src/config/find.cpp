#include "find.hpp"
#include "../socket/all.hpp"

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

MatchedServerResult matchedServer(const std::string &host, const Servers &servers, const Sd &sd)
{
    const PortNumberResult portNumberResult = portNumber(sd);
    if (portNumberResult.success)
    {
        const unsigned int port = portNumberResult.value;
        const Servers serversWithPort = filter(servers, port);
        if (serversWithPort.size() > 0)
        {
            const Servers serversWithPortAndName = filter(serversWithPort, host);
            if (serversWithPortAndName.size() > 0)
            {
                return MatchedServerResult::Success(serversWithPortAndName[0]);
            }
            else
            {
                return MatchedServerResult::Success(serversWithPort[0]);
            }
        }
        else
        {
            return MatchedServerResult::Error(BAD_REQUEST_RESPONSE);
        }
    }
    else
    {
        return MatchedServerResult::Error(portNumberResult.error);
    }
}

Location matchedLocation(const std::string &target, const std::vector<const Location> &locations)
{
    unsigned int matchedIndex = 0;
    for (unsigned int i = 1; i < locations.size(); i++)
    {
        if (utils::lengthOfPrefixMatch(target, locations[i].path) >
            utils::lengthOfPrefixMatch(target, locations[matchedIndex].path))
            matchedIndex = i;
    }
    return locations[matchedIndex];
}
