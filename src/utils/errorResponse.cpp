#include ".hpp"

namespace utils
{
HttpResponse generateErrorResponse(const int statusCode, const Server &server)
{
    const FileContentResult fileContentResult = utils::fileContent(server.errorPages.at(statusCode));
    if (fileContentResult.success)
    {
        return HttpResponse(statusCode, fileContentResult.value, utils::contentType(server.errorPages.at(statusCode)));
    }

    if (statusCode == SERVER_ERROR)
        return SERVER_ERROR_RESPONSE;
    return BAD_REQUEST_RESPONSE;
}
} // namespace utils
