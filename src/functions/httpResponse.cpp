#include "../all.hpp"

HttpResponse getRedirectHttpResponse(const HttpRequest &httpRequest, const std::string &redirectUrl)
{
    return HttpResponse(httpRequest.sd, REDIRECTION, "", "", redirectUrl);
}

HttpResponse getErrorHttpResponse(const HttpRequest &httpRequest, const int statusCode)
{
    const std::string path = getServer(httpRequest).errorPages.at(statusCode);
    const FileContentResult fileContentResult = utils::fileContent(path);
    if (fileContentResult.success)
    {
        // custom error page
        return HttpResponse(httpRequest.sd, statusCode, fileContentResult.value, utils::contentType(path));
    }
    else
    {
        // default error page
        if (statusCode == SERVER_ERROR)
            return HttpResponse(httpRequest.sd, SERVER_ERROR, SERVER_ERROR_BODY, CONTENT_TYPE_HTML);
        else if (statusCode == BAD_REQUEST)
            return HttpResponse(httpRequest.sd, BAD_REQUEST, BAD_REQUEST_BODY, CONTENT_TYPE_HTML);
    }
}

std::string stringify(const HttpResponse &response)
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
