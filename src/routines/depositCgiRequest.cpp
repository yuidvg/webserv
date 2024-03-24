#include "../all.hpp"

bool depositCgiRequest(const CgiRequest &cgiRequest)
{
    if (appendOutbound(cgiRequest.destinationSd, cgiRequest.body))
    {
        return true;
    }
    else
    {
        std::cerr << "Failed to deposit CGI request" << std::endl;
        return false;
    }
}
