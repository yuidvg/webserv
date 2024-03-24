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

bool appendOutbound(int sd, std::string content)
{
    FindSocketBufferResult findSocketBufferResult = findSocketBuffer(sd);
    if (findSocketBufferResult.success)
    {
        SocketBuffer &socketBuffer = findSocketBufferResult.value;
        socketBuffer.appendOutbound(content);
    }
    else
    {
        return false;
    }
}
