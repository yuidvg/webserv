#include "../all.hpp"

namespace utils
{

FileContentResult fileContent(const std::string &path)
{
    std::ifstream ifs(path);
    if (!ifs.is_open())
        return (FileContentResult::Error(BAD_REQUEST));

    std::string fileContent;
    std::string line;

    while (std::getline(ifs, line))
    {
        fileContent += line;
        fileContent.push_back('\n');
    }
    return (FileContentResult::Success(fileContent));
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

bool isDirectory(const std::string &path)
{
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0)
    {
        return false;
    }
    return S_ISDIR(statbuf.st_mode);
}

Option<Socket> createFile(const std::string &fileName, const std::string &path)
{
    std::string fullPath = path + fileName;
    const int fd = open(fullPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_NONBLOCK, 0644);
    if (fd >= 0)
    {
        if (utils::registerEvent(fd, EVFILT_WRITE))
        {
            const Socket &newFileSocket = Socket(fd, FILE_FD);
            SOCKETS.insert(newFileSocket);
            return Option<Socket>(newFileSocket);
        }
        else
        {
            close(fd);
            return Option<Socket>();
        }
    }
    else
        return Option<Socket>();
}

bool writeToFile(const std::string &path, const std::string &content)
{
    std::ofstream ofs(path);

    if (ofs.is_open())
    {
        ofs << content;
        return true;
    }
    else
    {
        return false;
    }
}
} // namespace utils
