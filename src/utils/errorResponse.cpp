#include ".hpp"

namespace utils
{
HttpResponse generateErrorResponse(const int statusCode, const std::string &path)
{
    const FileContentResult fileContentResult = utils::fileContent(path);
    if (fileContentResult.success)
    {
        return HttpResponse(statusCode, fileContentResult.value, utils::contentType(path));
    }
    if (statusCode == SERVER_ERROR)
        return SERVER_ERROR_RESPONSE;
    return BAD_REQUEST_RESPONSE;
}
} // namespace utils
