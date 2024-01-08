#include "HttpResponse.hpp"

size_t createResponse(char *buffer, size_t size, int status)
{
    std::string response = "HTTP/1.1 ";
    std::string statusStr = std::to_string(status);
    response += statusStr;
    response += " ";
    response += getStatusMessage(status);
    response += CRLF;
    response += "Content-Length: ";
    response += std::to_string(size);
    response += CRLF;
    response += "Content-Type: text/html\r\n";
    response += CRLF;
    response += buffer;
    std::memcpy(buffer, response.c_str(), response.size());
    return response.size();
}
