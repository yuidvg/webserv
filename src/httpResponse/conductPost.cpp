#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

static HttpResponse writeToFile(const std::string &path, const std::string &fileContent, const Server &server)
{
    std::ofstream ofs(path);
    if (ofs.is_open())
    {
        ofs << fileContent;
        return (HttpResponse(SUCCESS, fileContent, "text/html"));
    }
    else
    {
        return (utils::generateErrorResponse(BAD_REQUEST, server));
    }
}

HttpResponse conductPost(const HttpRequest &request, const Location &location, const Server &server)
{
    const std::string targetFilePath = resolvePath(location.uploadPath, request.target);

    const IsDirectoryResult isDirectoryResult = utils::isDirectory(location.uploadPath);
    if (!isDirectoryResult.success)
        return isDirectoryResult.error;
    if (!isDirectoryResult.value)
        return utils::generateErrorResponse(isDirectoryResult.error, server);
    const std::string fileName = std::string(utils::removeCharacter(request.target, '/') + ".txt");
    if (!utils::createFile(fileName, location.uploadPath))
        return utils::generateErrorResponse(BAD_REQUEST, server);
    const std::string fullFilePath = resolvePath(location.uploadPath, fileName);
    return writeToFile(fullFilePath, request.body, server);
}
