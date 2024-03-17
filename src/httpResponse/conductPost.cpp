#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

static HttpResponse writeToFile(const std::string &path, const std::string &fileContent, const ErrorPages &errorPages)
{
    std::ofstream ofs(path);
    if (ofs.is_open())
    {
        ofs << fileContent;
        return (HttpResponse(SUCCESS, fileContent, "text/html"));
    }
    else
    {
        return (errorPages.at(BAD_REQUEST));
    }
}

HttpResponse conductPost(const HttpRequest &request, const Location &location, const ErrorPages &errorPages)
{
    const std::string targetFilePath = resolvePath(location.uploadPath, request.target);

    const IsDirectoryResult isDirectoryResult = utils::isDirectory(location.uploadPath);
    if (!isDirectoryResult.success)
        return isDirectoryResult.error;
    if (!isDirectoryResult.value)
        return errorPages.at(isDirectoryResult.error);
    const std::string fileName = std::string(utils::removeCharacter(request.target, '/') + ".txt");
    if (!utils::createFile(fileName, location.uploadPath))
        return errorPages.at(BAD_REQUEST);
    const std::string fullFilePath = resolvePath(location.uploadPath, fileName);
    return writeToFile(fullFilePath, request.body, errorPages);
}
