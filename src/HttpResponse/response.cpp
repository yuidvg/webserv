#include "response.hpp"

size_t createResponse(char *buffer, size_t size, int status)
{
    std::string response = "HTTP/1.1 ";
    std::string statusStr = std::to_string(status);
    response += statusStr;
    response += " ";
    response += getStatusMessage(status);
    response += "Content-Length: ";
    response += std::to_string(size);
    response += "Content-Type: text/html\n";
    response += buffer;
    std::memcpy(buffer, response.c_str(), response.size());
    return response.size();
}

HttpResponse response(const ParsedRequest request, const )
{
    HttpResponse res;
    res.statusCode = 200;
    res.headers["Content-Type"] = "text/html";
    res.body = "<h1>hello world</h1>";
    return res;
}