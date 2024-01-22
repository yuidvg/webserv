#include "../socket/all.hpp"
#include "all.hpp"

namespace utils
{

static Servers filter(const Servers servers, const unsigned int port)
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

static Server filter(const std::string uri, const Servers servers)
{
    std::vector<int> matchedIndexes;
    for (unsigned int i = 0; i < matchedIndexes.size(); i++)
    {
        // ToDo: std::string hostName(const std::string uri)
        if (servers[matchedIndexes[i]].name == uri)
        {
            return (servers[matchedIndexes[i]]);
        }
    }
}

MatchedServerResult matchedServer(const std::string uri, const Servers servers, const Sd sd)
{
    const PortNumberResult portNumberResult = portNumber(sd);
    if (portNumberResult.success)
    {
        const unsigned int port = portNumberResult.value;
        const Servers serversWithPort = filter(servers, port);
        const Server serversWithPortAndName = filter(uri, serversWithPort);
        return MatchedServerResult::Success(serversWithPortAndName);
    }
    else
    {
        return MatchedServerResult::Error(portNumberResult.error);
    }
}
} // namespace utils
