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
