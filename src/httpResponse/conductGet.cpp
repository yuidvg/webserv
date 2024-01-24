#include ".hpp"
#include "autoindex/.hpp"
#include "matchedDirective/.hpp"

namespace
{
std::string contentType(const std::string &path)
{
    const std::string extension = path.substr(path.find_last_of(".") + 1);
    if (extension == "html" || extension == "htm")
        return "text/html";
    else if (extension == "css")
        return "text/css";
    else if (extension == "js")
        return "application/javascript";
    else if (extension == "jpg" || extension == "jpeg")
        return "image/jpeg";
    else if (extension == "png")
        return "image/png";
    else if (extension == "json")
        return "application/json";
    else if (extension == "xml")
        return "application/xml";
    else if (extension == "pdf")
        return "application/pdf";
    else if (extension == "zip")
        return "application/zip";
    else if (extension == "mp3")
        return "audio/mpeg";
    else if (extension == "mp4")
        return "video/mp4";
    else if (extension == "wav")
        return "audio/wav";
    else if (extension == "avi")
        return "video/x-msvideo";
    else if (extension == "mov")
        return "video/quicktime";
    else if (extension == "doc")
        return "application/msword";
    else if (extension == "xls")
        return "application/vnd.ms-excel";
    else if (extension == "ppt")
        return "application/vnd.ms-powerpoint";
    else if (extension == "gif")
        return "image/gif";
    else if (extension == "bmp")
        return "image/bmp";
    else if (extension == "ico")
        return "image/vnd.microsoft.icon";
    else if (extension == "svg")
        return "image/svg+xml";
    else
        return "text/plain";
}
} // namespace

HttpResponse conductGet(const HttpRequest &request, const Location &location)
{
    const std::string targetResourcePath = comply(request.target, location);
    const IsDirectoryResult isDirectoryResult = utils::isDirectory(targetResourcePath);
    if (isDirectoryResult.success)
    {
        const bool isDirectory = isDirectoryResult.value;
        if (isDirectory)
        {
            if (location.autoindex)
            {
                const DirectoryListHtmlResult directoryListHtmlResult = directoryListHtml(targetResourcePath);
                return directoryListHtmlResult.success
                           ? HttpResponse(SUCCESS,
                                          Headers(utils::contentType(".html"),
                                                  utils::toString(directoryListHtmlResult.value.length())),
                                          directoryListHtmlResult.value)
                           : BAD_REQUEST_RESPONSE;
            }
            else
            {
                return BAD_REQUEST_RESPONSE;
            }
        }
        else // when targetResourcePath is assumed to be a file.
        {
            const FileContentResult fileContentResult = utils::content(targetResourcePath);
            return fileContentResult.success ? HttpResponse(SUCCESS,
                                                            Headers(utils::contentType(targetResourcePath),
                                                                    utils::toString(fileContentResult.value.length())),
                                                            fileContentResult.value)
                                             : BAD_REQUEST_RESPONSE;
        }
    }
    else
    {
        return isDirectoryResult.error;
    }
}
