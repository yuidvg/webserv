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
CgiRequestOrHttpResponse processLocalRedirectResponse(const CgiResponse &cgiResponse, const HttpRequest &request,
                                                      const Client &client, const ErrorPages &errorPages)
{
    const HttpRequest redirectRequest(request.method, cgiResponse.location, request.headers, request.body,
                                      request.host);
    return processHttpRequest(redirectRequest, client, errorPages);
}

bool isClientRedirectResponse(const CgiResponse &cgiResponse)
{
    return cgiResponse.location.size() > 0 && utils::isAbsoluteUri(cgiResponse.location) &&
           cgiResponse.contentType.size() == 0 && cgiResponse.status == 0 && cgiResponse.body.size() == 0 &&
           cgiResponse.otherHeaders.size() == 0;
}
HttpResponse processClientRedirectResponse(const CgiResponse &cgiResponse)
{
    return HttpResponse(302, "", "", cgiResponse.location);
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

CgiRequestOrHttpResponse processCgiResponse(const CgiResponse &cgiResponse, const HttpRequest &request, Client &client,
                                            const ErrorPages &errorPages)
{
    if (isClientRedirectWithDocumentResponse(cgiResponse))
    {
        return CgiRequestOrHttpResponse::Right(processClientRedirectWithDocumentResponse(cgiResponse));
    }
    else if (isClientRedirectResponse(cgiResponse))
    {
        return CgiRequestOrHttpResponse::Right(processClientRedirectResponse(cgiResponse));
    }
    else if (isLocalRedirectResponse(cgiResponse))
    {
        return processLocalRedirectResponse(cgiResponse, request, client, errorPages);
    }
    else if (isDocumentResponse(cgiResponse))
    {
        return CgiRequestOrHttpResponse::Right(processDocumentResponse(cgiResponse));
    }
    else
    {
        return CgiRequestOrHttpResponse::Right(errorPages.at(SERVER_ERROR));
    }
}
