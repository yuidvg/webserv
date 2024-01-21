#include "../webserv.hpp"

std::vector<std::string> readDirectory(const std::string& path)
{
    std::vector<std::string> files;
    DIR* dir = opendir(path.c_str());

    if (dir != NULL)
    {
        struct dirent* entry;
        while ((entry = readdir(dir)) != NULL)
            files.push_back(entry->d_name);
        closedir(dir);
    }
    return files;
}

std::string createHtmlList(const std::vector<std::string>& files, const std::string& path)
{
    std::ostringstream html;
    html << "<html><head><title>Index of " << path << "</title></head><body>";
    html << "<h1>Index of " << path << "</h1><ul>";

    for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
        html << "<li><a href=\"" << *it << "\">" << *it << "</a></li>";

    html << "</ul></body></html>";
    return html.str();
}

DirectoryListHtmlResult directoryListHtml(const std::string& path)
{
    std::vector<std::string> files = readDirectory(path);

    if (files.empty())
        return DirectoryListHtmlResult::Error("<html><body><p>Directory not found.</p></body></html>");

    return DirectoryListHtmlResult::Success(createHtmlList(files, path));
}


GetIndexFilePathResult getIndexFilePath(const std::string& directoryPath, const std::string& indexFileName)
{
    std::string indexPath = directoryPath + "/" + indexFileName;
    std::ifstream file(utils::toChar(indexPath));
    if (file.good())
        return GetIndexFilePathResult::Success(indexPath);

    return GetIndexFilePathResult::Error("index file not found");
}

std::string getContentForRequest(const std::string& path, const Location &location)
{
    GetIndexFilePathResult indexFilePath = getIndexFilePath(path, location.index);

    if (indexFilePath.success)
    {
        return indexFilePath.value;
    }
    if (location.autoindex)
    {
        DirectoryListHtmlResult html = directoryListHtml(path);
        if (html.success)
        {
            return html.value;
        }
    }
    return "400 BAD REQUEST"; // カスタムエラーページに置き換えることが可能
}

