#include "../all.hpp"

void eraseSocketBuffer(const int sd)
{
    for (SocketBuffers::iterator it = SOCKET_BUFFERS.begin(); it != SOCKET_BUFFERS.end();)
    {
        if (it->sd == sd)
        {
            SOCKET_BUFFERS.erase(it);
            break;
        }
        else
        {
            ++it;
        }
    }
}

void wipeCgi(const pid_t cgiPid, const int sd)
{
    // Remove the socket buffer
    eraseSocketBuffer(sd);

    // Close the socket
    close(sd);

    // Kill the Cgi process
    int wstats;
    if (waitpid(cgiPid, &wstats, WNOHANG) == 0)
    {
        kill(cgiPid, SIGKILL);
        waitpid(cgiPid, &wstats, 0);
    }
}

