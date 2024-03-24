#include "../all.hpp"

namespace
{
bool isDocumentResponse(const CgiResponse &cgiResponse)
{
    return cgiResponse.contentType.size() > 0;
}

HttpResponse processDocumentResponse(const CgiResponse &cgiResponse)
{
    const HttpResponse response(utils::isStatusInRange(cgiResponse.status) ? cgiResponse.status : 200,
                                cgiResponse.body);
    return response;
}

bool isLocalRedirectResponse(const CgiResponse &cgiResponse)
{
    return cgiResponse.location.size() > 0 && utils::isAbsolutePath(cgiResponse.location);
}

HttpRequest processLocalRedirectResponse(const CgiResponse &cgiResponse, const HttpRequest &httpRequest,
                                         const ErrorPages &errorPages)
{
    return HttpRequest(httpRequest.sd, httpRequest.serverPort, httpRequest.clientIp, httpRequest.host,
                       httpRequest.method, cgiResponse.location, httpRequest.headers, httpRequest.body);
}

bool isClientRedirectResponse(const CgiResponse &cgiResponse)
{
    return cgiResponse.location.size() > 0 && utils::isAbsoluteUri(cgiResponse.location) &&
           cgiResponse.contentType.size() == 0 && cgiResponse.status == 0 && cgiResponse.body.size() == 0 &&
           cgiResponse.otherHeaders.size() == 0;
}
HttpResponse processClientRedirectResponse(const CgiResponse &cgiResponse)
{
    return HttpResponse(, 302, "", "", cgiResponse.location);
}

bool isClientRedirectWithDocumentResponse(const CgiResponse &cgiResponse)
{
    return cgiResponse.status == 302 && cgiResponse.location.size() > 0 && cgiResponse.contentType.size() > 0 &&
           utils::isAbsoluteUri(cgiResponse.location);
}
HttpResponse processClientRedirectWithDocumentResponse(const CgiResponse &cgiResponse)
{
    return HttpResponse(cgiResponse.status, cgiResponse.body, cgiResponse.contentType, cgiResponse.location);
}
} // namespace

HttpMessage processCgiResponse(const CgiResponse &cgiResponse, const HttpRequest &httpRequest)
{
    if (isClientRedirectWithDocumentResponse(cgiResponse))
    {
        return HttpMessage::Right(processClientRedirectWithDocumentResponse(cgiResponse));
    }
    else if (isClientRedirectResponse(cgiResponse))
    {
        return HttpMessage::Right(processClientRedirectResponse(cgiResponse));
    }
    else if (isLocalRedirectResponse(cgiResponse))
    {
        return processLocalRedirectResponse(cgiResponse, httpRequest);
    }
    else if (isDocumentResponse(cgiResponse))
    {
        return HttpMessage::Right(processDocumentResponse(cgiResponse));
    }
    else
    {
        return HttpMessage::Right(getErrorResponse(httpRequest, SERVER_ERROR));
    }
}
