#include "../all.hpp"

bool killCgi(const Socket &cgiSocket)
{
    if (cgiSocket.descriptor != -1 && close(cgiSocket.descriptor) != -1)
    {
        int wstats;
        if (waitpid(cgiSocket.opponentPid, &wstats, WNOHANG) == 0)
        {
            kill(cgiSocket.opponentPid, SIGKILL);
            waitpid(cgiSocket.opponentPid, &wstats, 0);
        }
        return true;
    }
    return false;
}
