#include "../autoindex/.hpp"
#include "../httpRequestAndConfig/.hpp"
#include ".hpp"

HttpResponse conductPost(const HttpRequest &request, const Location &location)
{
    const std::string targetResourcePath = location.uploadPath;
    const IsDirectoryResult isDirectoryResult = utils::isDirectory(targetResourcePath);
    if (isDirectoryResult.success)
    {
        const bool isDirectory = isDirectoryResult.value;
        if (isDirectory)
        {
            const std::string fileName = std::string(utils::removeCharacter(request.target, '/') + ".txt");
            if (!utils::createFile(fileName))
                return BAD_REQUEST_RESPONSE;
            return utils::writeToFile(resolvePath(location.uploadPath, fileName), request.body);
        }
        else // when uri.extraPath is assumed to be a file.
        {
            return BAD_REQUEST_RESPONSE;
        }
    }
    else
    {
        return isDirectoryResult.error;
    }
}
