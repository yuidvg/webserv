#include "../all.hpp"

namespace
{
bool isDocumentResponse(const CgiResponse &cgiResponse)
{
    return cgiResponse.contentType.size() > 0;
}

HttpResponse processDocumentResponse(const CgiResponse &cgiResponse)
{
    return HttpResponse(cgiResponse.httpRequest.sd,
                        utils::isStatusInRange(cgiResponse.status) ? cgiResponse.status : SUCCESS, cgiResponse.body);
}

bool isLocalRedirectResponse(const CgiResponse &cgiResponse)
{
    return cgiResponse.location.size() > 0 && utils::isAbsolutePath(cgiResponse.location);
}

HttpRequest processLocalRedirectResponse(const CgiResponse &cgiResponse)
{
    const HttpRequest &httpRequest = getHttpRequest(cgiResponse.cgiSd);
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
    const HttpRequest &httpRequest = getHttpRequest(cgiResponse.cgiSd);
    return HttpResponse(httpRequest.sd, 302, "", "", cgiResponse.location);
}

bool isClientRedirectWithDocumentResponse(const CgiResponse &cgiResponse)
{
    return cgiResponse.status == 302 && cgiResponse.location.size() > 0 && cgiResponse.contentType.size() > 0 &&
           utils::isAbsoluteUri(cgiResponse.location);
}
HttpResponse processClientRedirectWithDocumentResponse(const CgiResponse &cgiResponse)
{
    const HttpRequest &httpRequest = getHttpRequest(cgiResponse.cgiSd);
    return HttpResponse(httpRequest.sd, cgiResponse.status, cgiResponse.body, cgiResponse.contentType,
                        cgiResponse.location);
}
} // namespace

HttpMessage processCgiResponse(const CgiResponse &cgiResponse)
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
        return HttpMessage::Left(processLocalRedirectResponse(cgiResponse));
    }
    else if (isDocumentResponse(cgiResponse))
    {
        return HttpMessage::Right(processDocumentResponse(cgiResponse));
    }
    else
    {
        return HttpMessage::Right(getErrorHttpResponse(getHttpRequest(cgiResponse.cgiSd), SERVER_ERROR));
    }
}
