#include "../all.hpp"

bool eraseSocketBuffer(const int sd)
{
    for (SocketBuffers::iterator it = SOCKET_BUFFERS.begin(); it != SOCKET_BUFFERS.end(); ++it)
    {
        if (it->sd == sd)
        {
            if (close(it->sd) == 0)
            {
                utils::unregisterEvent(it->sd);
                it = SOCKET_BUFFERS.erase(it);
                break;
            }
            else
            {
                utils::printError("Failed to close socket");
                return false;
            }
        }
    }
    return true;
}

void addSocketBuffer(const int sd)
{
    for (SocketBuffers::iterator it = SOCKET_BUFFERS.begin(); it != SOCKET_BUFFERS.end(); ++it)
    {
        if (it->sd == sd)
            return;
    }
    SOCKET_BUFFERS.push_back(SocketBuffer(sd));
}

void wipeCgi(const pid_t cgiPid, const int sd)
{
    if (eraseSocketBuffer(sd))
    {
        // Kill the Cgi process
        int wstats;
        if (waitpid(cgiPid, &wstats, WNOHANG) == 0)
        {
            kill(cgiPid, SIGKILL);
            waitpid(cgiPid, &wstats, 0);
        }
    }
}
