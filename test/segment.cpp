#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
std::vector<std::string> segment(const std::string &path)
{
    std::vector<std::string> segments;
    std::string::size_type slashPos = path.find('/');
    while (slashPos != std::string::npos)
    {
        std::string::size_type nextSlashPos = path.find('/', slashPos + 1);
        segments.push_back(path.substr(slashPos + 1, nextSlashPos - slashPos - 1));
        slashPos = nextSlashPos;
    }
    return segments;
}
std::vector< std::string> tokenize(const std::string &original, const char &delim)
{
    std::vector< std::string> tokens;
    std::stringstream ss(original);
    std::string token;
    while (std::getline(ss, token, delim))
        if (!token.empty())
            tokens.push_back(token);
    return tokens;
}
} // namespace

int main()
{
    std::vector<std::string> segments = segment("test/to/segment.cpp?test=1");
    std::cout << "size: " << segments.size() << std::endl;
    for (std::vector<std::string>::iterator it = segments.begin(); it != segments.end(); ++it)
    {
        std::cout << *it << std::endl;
    }

    std::vector< std::string> tokens = tokenize("test/to/segment.cpp?test=1", '/');
    return 0;
}
