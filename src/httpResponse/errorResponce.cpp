#include ".hpp"

static HttpResponse generateErrorResponse(const HttpResponse &httpResponse, const Server &server)
{
    const FileContentResult fileContentResult = utils::fileContent(server.errorPages.at(httpResponse.statusCode));
    if (fileContentResult.success)
    {
        return HttpResponse(httpResponse.statusCode,
                            Headers(utils::contentType(server.errorPages.at(httpResponse.statusCode)),
                                    utils::toString(fileContentResult.value.length())),
                            fileContentResult.value);
    }

    if (httpResponse.statusCode == BAD_REQUEST)
        return BAD_REQUEST_RESPONSE;
    else
        return SERVER_ERROR_RESPONSE;
}

HttpResponse provideErrorResponse(const HttpResponse &httpResponse, const Server &server)
{
    if (httpResponse.statusCode == BAD_REQUEST && server.errorPages.find(BAD_REQUEST) != server.errorPages.end())
    {
        return generateErrorResponse(httpResponse, server);
    }
    else if (httpResponse.statusCode == SERVER_ERROR && server.errorPages.find(SERVER_ERROR) != server.errorPages.end())
    {
        return generateErrorResponse(httpResponse, server);
    }
    return httpResponse;
}
