#include ".hpp"

namespace utils
{
ErrorPage generateErrorPage(const int statusCode, const std::string &path)
{
    const FileContentResult fileContentResult = utils::fileContent(path);
    if (fileContentResult.success)
    {
        return ErrorPage(statusCode, HttpResponse(statusCode, fileContentResult.value, utils::contentType(path)));
    }
    if (statusCode == SERVER_ERROR)
        return ErrorPage(statusCode, SERVER_ERROR_RESPONSE);
    return ErrorPage(BAD_REQUEST, BAD_REQUEST_RESPONSE);
}
} // namespace utils
