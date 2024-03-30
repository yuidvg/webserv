#include "../all.hpp"

void appendOutbound(int sd, std::string content)
{
    getSocketBuffer(sd).appendOutbound(content);
}
void depositCgiRequest(const CgiRequest &cgiRequest)
{
    appendOutbound(cgiRequest.destinationSd, cgiRequest.body);
}
