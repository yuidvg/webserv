#include ".hpp"

namespace
{
std::string recieveResponse(int fd)
{
    char buffer[1024];
    std::string response;
    int n;
    while ((n = read(fd, buffer, 1024)) > 0)
    {
        buffer[n] = '\0';
        response += buffer;
    }
    return response;
}

} // namespace
