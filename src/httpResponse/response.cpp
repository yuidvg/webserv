#include ".hpp"
#include "../cgiRequest/.hpp"
#include "../httpRequestAndConfig/.hpp"


std::string responseText(const HttpResponse &response)
{
    std::vector<std::string> headers;
    const std::string statusLine = SERVER_PROTOCOL + " " + std::to_string(response.statusCode);
    headers.push_back(statusLine);
    if (response.contentType.size() > 0)
    {
        headers.push_back("Content-Type: " + response.contentType);
    }
    if (response.body.size() >= 0)
    {
        headers.push_back("Content-Length: " + std::to_string(response.body.size()));
    }
    if (response.location.size() > 0)
    {
        headers.push_back("Location: " + response.location);
    }
    if (response.allow.size() > 0)
    {
        headers.push_back("Allow: " + response.allow);
    }
    const std::string headersText = utils::join(headers, CRLF);

    std::vector<std::string> blocks;
    blocks.push_back(headersText);
    blocks.push_back("");
    blocks.push_back(response.body);
    return utils::join(blocks, CRLF);
}
