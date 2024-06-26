#include "../all.hpp"

namespace
{
std::vector<std::string> readDirectory(const std::string &path)
{
    std::vector<std::string> files;
    DIR *dir = opendir(path.c_str());

    if (dir != NULL)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL)
            files.push_back(entry->d_name);
        closedir(dir);
    }
    return files;
}

std::string createHtmlList(const std::vector<std::string> &files, const std::string &path)
{
    std::ostringstream html;
    html << "<html><head><title>Index of " << path << "</title></head><body>";
    html << "<h1>Index of " << path << "</h1><ul>";

    for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
        html << "<li><a href=\"" << *it << "\">" << *it << "</a></li>";

    html << "</ul></body></html>";
    return html.str();
}
} // namespace

DirectoryListHtmlResult directoryListHtml(const std::string &path)
{
    std::vector<std::string> files = readDirectory(path);

    if (files.empty())
    {
        std::string body = "<html><body><p>Directory not found.</p></body></html>";
        return DirectoryListHtmlResult::Error(BAD_REQUEST);
    }

    return DirectoryListHtmlResult::Success(createHtmlList(files, path));
}
