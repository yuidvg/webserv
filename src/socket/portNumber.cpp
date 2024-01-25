#include ".hpp"

PortNumberResult portNumber(const Socket sd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    if (getsockname(sd, (struct sockaddr *)&addr, &len) == 0)
    {
        return PortNumberResult::Success(ntohs(addr.sin_port));
    }
    else
    {
        return PortNumberResult::Error(SERVER_ERROR_RESPONSE);
    }
}
