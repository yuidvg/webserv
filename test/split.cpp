#include <string>
#include <vector>

namespace
{
std::vector<std::string> split(const std::string &original, const std::string &delim)
{
    std::vector<std::string> tokens;
    std::string::size_type start = 0;
    std::string::size_type end = 0;
    while ((end = original.find(delim, start)) != std::string::npos)
    {
        const std::string token = original.substr(start, end - start);
        if (!token.empty())
            tokens.push_back(token);
        start = end + delim.length();
    }
    const std::string lastToken = original.substr(start);
    if (!lastToken.empty())
        tokens.push_back(lastToken);
    return tokens;
}
} // namespace

int main()
{
    std::string original = "a,b,c,d,e\n\nhoge,fuga,piyo\n\nfoo,bar,baz\n\n";
    std::string delim = "\n\n";
    std::vector<std::string> tokens = split(original, delim);
    return 0;
}
