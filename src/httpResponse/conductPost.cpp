#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

HttpResponse conductPost(const HttpRequest &request, const Location &location)
{
    const std::string targetFilePath = resolvePath(location.uploadPath, request.target);

    const IsDirectoryResult isDirectoryResult = utils::isDirectory(location.uploadPath);
    if (!isDirectoryResult.success)
        return isDirectoryResult.error;
    if (!isDirectoryResult.value)
        return BAD_REQUEST_RESPONSE;
    const std::string fileName = std::string(utils::removeCharacter(request.target, '/') + ".txt");
    if (!utils::createFile(fileName, location.uploadPath))
        return BAD_REQUEST_RESPONSE;
    const std::string fullFilePath = resolvePath(location.uploadPath, fileName);
    return utils::writeToFile(fullFilePath, request.body);
}
