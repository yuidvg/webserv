#include "../all.hpp"

void depositCgiRequest(const CgiRequest &cgiRequest)
{
    if (!setOutbound(cgiRequest.destinationSd, cgiRequest.scriptPath.c_str()))
    {
        std::cerr << "Failed to deposit CGI request" << std::endl;
    }
}
