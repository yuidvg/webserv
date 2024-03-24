#include "../all.hpp"

namespace
{

} // namespace

void vacant(const pid_t cgiPid, const int sd)
{
    // Remove the socket buffer
    eraseSocketBuffer(sd);
    // Kill the Cgi process
    int wstats;
    if (waitpid(cgiPid, &wstats, WNOHANG) == 0)
    {
        kill(cgiPid, SIGKILL);
        waitpid(cgiPid, &wstats, 0);
    }
}
