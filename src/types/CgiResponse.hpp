#pragma once

#include <string>

enum CGI_RESPONSE_TYPE
{
    DOCUMENT,
    LOCAL_REDIRECT,
    CLIENT_REDIRECT,
    CLIENT_REDIRECT_WITH_DOCUMENT
};

struct CgiResponse
{
    const CGI_RESPONSE_TYPE type;
    const std::string contentType;
    const std::string location;
    const unsigned int status;
    const std::string otherHeaders;
    const std::string body;

    CgiResponse(const CGI_RESPONSE_TYPE &type, const std::string &contentType, const std::string &location,
                const unsigned int status, const std::string &otherHeaders, const std::string &body)
        : type(type), contentType(contentType), location(location), status(status), otherHeaders(otherHeaders),
          body(body)
    {
    }
    CgiResponse() : type(DOCUMENT), contentType(""), location(""), status(0), otherHeaders(""), body("")
    {
    }
};


CgiResponse cgiResponse = parseCgiResponse();


if (isDocumentResponse(cgiResponse))
{
    return processCgiDocumentResponse(cgiResponse);
}
else if (isLocalRedirectResponse(cgiResponse))
{
    return processLocalRedirectResponse(cgiResponse);
}
else if (isClientRedirectResponse(cgiResponse))
{
    return processClientRedirectResponse(cgiResponse);
}
else if (isClientRedirectWithDocumentResponse(cgiResponse))
{
    return processClientRedirectWithDocumentResponse(cgiResponse);
}
else
{
    return SERVER_ERROR_RESPONSE;
}

HttpResponse processCgiDocumentResponse(cgiResponse);