#include "../all.hpp"

HttpResponseOrEventData conductPost(const HttpRequest &httpRequest)
{
    const Location &location = getLocation(httpRequest);
    const std::string targetFilePath = utils::concatPath(location.uploadPath, httpRequest.target);
    if (utils::isDirectory(location.uploadPath))
    {
        const std::string fileName = httpRequest.target.substr(httpRequest.target.find_last_of('/') + 1);
        const int fd = utils::createFile(fileName, location.uploadPath);
        if (fd >= 0)
        {
            const EventData eventData(Socket(fd, FILE_FD), httpRequest.body);
            return HttpResponseOrEventData::Right(eventData);
        }
        else
            return HttpResponseOrEventData::Left(HttpResponse(getErrorHttpResponse(httpRequest, BAD_REQUEST)));
    }
    return HttpResponseOrEventData::Left(HttpResponse(getErrorHttpResponse(httpRequest, BAD_REQUEST)));
}

HttpResponse conductDelete(const HttpRequest &httpRequest)
{
    const std::string path = httpRequest.target.substr(1);                   // upload/hogehoge
    const std::string targetFile = path.substr(path.find_first_of('/') + 1); // hogehoge
    const Location location = getLocation(httpRequest);
    const std::string relativePath =
        location.root.empty() ? "./" + location.uploadPath + '/' + targetFile : "./" + location.root + '/' + targetFile;
    if (remove(relativePath.c_str()) == 0)
        return (HttpResponse(httpRequest.socket.descriptor, SUCCESS, httpRequest.target, "text/html"));
    else
    {
        strerror(errno);
        return getErrorHttpResponse(httpRequest, BAD_REQUEST);
    }
}

HttpResponse conductGet(const HttpRequest &httpRequest, const std::string &target)
{
    if (utils::isDirectory(target))
    {
        const Location &location = getLocation(httpRequest);
        if (location.autoindex)
        {
            const DirectoryListHtmlResult directoryListHtmlResult = directoryListHtml(target);
            return directoryListHtmlResult.success ? HttpResponse(httpRequest.socket.descriptor, SUCCESS,
                                                                  directoryListHtmlResult.value, "text/html")
                                                   : getErrorHttpResponse(httpRequest, BAD_REQUEST);
        }
        else if (location.index.size() > 0)
        {
            const std::string indexPath = target + '/' + location.index;
            const FileContentResult fileContentResult = utils::fileContent(indexPath);
            return fileContentResult.success ? HttpResponse(httpRequest.socket.descriptor, SUCCESS,
                                                            fileContentResult.value, utils::contentType(indexPath))
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
        return fileContentResult.success ? HttpResponse(httpRequest.socket.descriptor, SUCCESS, fileContentResult.value,
                                                        utils::contentType(target))
                                         : getErrorHttpResponse(httpRequest, BAD_REQUEST);
    }
}
