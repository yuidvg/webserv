#include ".hpp"

namespace utils
{

FileContentResult content(const std::string &path)
{
    std::ifstream ifs(path);
    if (!ifs.is_open())
        return (FileContentResult::Error(HttpResponse(BAD_REQUEST, Headers(), "File not found")));

    std::string content;
    std::string line;

    while (std::getline(ifs, line))
    {
        content += line;
        content.push_back('\n');
    }
    return (FileContentResult::Success(content));
}

HttpResponse writeToFile(const std::string &path, const std::string &content)
{
    std::ofstream ofs(path);
    if (ofs.is_open())
    {
        ofs << content;
        return (HttpResponse(SUCCESS, Headers(), ""));
    }
    else
    {
        return (BAD_REQUEST_RESPONSE);
    }
}


IsDirectoryResult isDirectory(const std::string &path)
{
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0)
    {
        return IsDirectoryResult::Error(SERVER_ERROR_RESPONSE);
    }
    return IsDirectoryResult::Success(S_ISDIR(statbuf.st_mode));
}
} // namespace utils
