#include "../all.hpp"

HttpResponse conductPost(const HttpRequest &httpRequest)
{
    const Location &location = getLocation(httpRequest);
    const std::string targetFilePath = resolvePath(location.uploadPath, httpRequest.target);
    if (utils::isDirectory(location.uploadPath))
    {
        const std::string fileName = std::string(utils::removeCharacter(httpRequest.target, '/') + ".txt");
        if (!utils::createFile(fileName, location.uploadPath))
            return getErrorResponse(httpRequest, BAD_REQUEST);
        const std::string fullFilePath = resolvePath(location.uploadPath, fileName);
        return utils::writeToFile(fullFilePath, httpRequest.body)
                   ? HttpResponse(httpRequest.sd, SUCCESS, "File created", "text/plain")
                   : getErrorResponse(httpRequest, BAD_REQUEST);
    }
    return getErrorResponse(httpRequest, BAD_REQUEST);
}

HttpResponse conductDelete(const HttpRequest &httpRequest)
{
    const std::string relativePath = httpRequest.target.substr(1);
    if (remove(relativePath.c_str()) == 0)
        return (HttpResponse(httpRequest.sd, SUCCESS, httpRequest.target, "text/html"));
    else
        return getErrorResponse(httpRequest, BAD_REQUEST);
}

HttpResponse conductGet(const HttpRequest &httpRequest)
{
    const Uri uri = segment(httpRequest);

    if (utils::isDirectory(uri.extraPath))
    {
        const Location &location = getLocation(httpRequest);
        if (location.autoindex)
        {
            const DirectoryListHtmlResult directoryListHtmlResult = directoryListHtml(uri.extraPath);
            return directoryListHtmlResult.success
                       ? HttpResponse(httpRequest.sd, SUCCESS, directoryListHtmlResult.value, "text/html")
                       : BAD_REQUEST_RESPONSE;
        }
        else if (location.index.size() > 0)
        {
            const std::string indexPath = uri.extraPath + location.index;
            const FileContentResult fileContentResult = utils::fileContent(indexPath);
            return fileContentResult.success
                       ? HttpResponse(httpRequest.sd, SUCCESS, fileContentResult.value, utils::contentType(indexPath))
                       : BAD_REQUEST_RESPONSE;
        }
        else
        {
            return getErrorResponse(httpRequest, BAD_REQUEST);
        }
    }
    else // when uri.extraPath is assumed to be a file.
    {
        const FileContentResult fileContentResult = utils::fileContent(uri.extraPath);
        return fileContentResult.success
                   ? HttpResponse(httpRequest.sd, SUCCESS, fileContentResult.value, utils::contentType(uri.extraPath))
                   : getErrorResponse(httpRequest, BAD_REQUEST);
    }
}
