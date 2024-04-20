#include "../all.hpp"

HttpResponse conductPost(const HttpRequest &httpRequest)
{
    const Location &location = getLocation(httpRequest);
    const std::string targetFilePath = utils::concatPath(location.uploadPath, httpRequest.target);
    if (utils::isDirectory(location.uploadPath))
    {
        const std::string fileName = std::string(utils::removeCharacter(httpRequest.target, '/') + ".txt");
        const int fd = utils::createFile(fileName, location.uploadPath);
        if (fd  >= 0)
        {
            SocketBuffer socketBufferForFileToWrite(fd);
            socketBufferForFileToWrite.appendOutbound(httpRequest.body);
            SOCKET_BUFFERS.push_back(socketBufferForFileToWrite);
            return HttpResponse(httpRequest.sd, SUCCESS, "File upload", "text/plain");
        }
        else
            return getErrorHttpResponse(httpRequest, BAD_REQUEST);
    }
    return getErrorHttpResponse(httpRequest, BAD_REQUEST);
}

HttpResponse conductDelete(const HttpRequest &httpRequest)
{
    const std::string relativePath = httpRequest.target.substr(1);
    if (remove(relativePath.c_str()) == 0)
        return (HttpResponse(httpRequest.sd, SUCCESS, httpRequest.target, "text/html"));
    else
        return getErrorHttpResponse(httpRequest, BAD_REQUEST);
}

HttpResponse conductGet(const HttpRequest &httpRequest, const std::string &target)
{
    if (utils::isDirectory(target))
    {
        const Location &location = getLocation(httpRequest);
        if (location.autoindex)
        {
            const DirectoryListHtmlResult directoryListHtmlResult = directoryListHtml(target);
            return directoryListHtmlResult.success
                       ? HttpResponse(httpRequest.sd, SUCCESS, directoryListHtmlResult.value, "text/html")
                       : getErrorHttpResponse(httpRequest, BAD_REQUEST);
        }
        else if (location.index.size() > 0)
        {
            const std::string indexPath = target + location.index;
            const FileContentResult fileContentResult = utils::fileContent(indexPath);
            return fileContentResult.success
                       ? HttpResponse(httpRequest.sd, SUCCESS, fileContentResult.value, utils::contentType(indexPath))
                       : getErrorHttpResponse(httpRequest, BAD_REQUEST);
        }
        else
        {
            return getErrorHttpResponse(httpRequest, BAD_REQUEST);
        }
    }
    else // when target is assumed to be a file.
    {
        const FileContentResult fileContentResult = utils::fileContent(target);
        return fileContentResult.success
                   ? HttpResponse(httpRequest.sd, SUCCESS, fileContentResult.value, utils::contentType(target))
                   : getErrorHttpResponse(httpRequest, BAD_REQUEST);
    }
}
