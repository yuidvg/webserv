#include "response.hpp"
const

    HttpResponse
    response(const ParsedRequestResult request, const Server server)
{
    std::map<std::string, std::string> headers;
    // Locate the requested resource
    std::string resourcePath = server.root + request.uri;
    std::ifstream ifs(resourcePath);
    if (!ifs.is_open())
    {
        // If the resource is not found, return a 404 response";
        HttpResponse response = HttpResponse(BAD_REQUEST, headers, "");
        return response;
    }
    const std::string content = utils::content(ifs);
    headers["Content-Type"] = utils::contentType(resourcePath);

    return HttpResponse(200, headers, content);
}

std::string makeResponseMessage(const HttpResponse &response)
{
    std::string httpResponse = "HTTP/1.1 " + std::to_string(response.statusCode) + "\r\n";
    for (std::map<std::string, std::string>::const_iterator it = response.headers.begin(); it != response.headers.end();
         ++it)
        httpResponse += it->first + ": " + it->second + "\r\n";
    httpResponse += "\r\n";
    httpResponse += response.body;
    return httpResponse;
}
