#include ".hpp"

namespace utils
{
bool isAbsolutePath(const std::string &path)
{
    return path.size() > 0 && path[0] == '/';
}

bool isAbsoluteUri(const std::string &uri)
{
    const std::size_t schemeEnd = uri.find(URI_SCHEME_DELIMITER);
    if (schemeEnd == std::string::npos)
    {
        return false; // No scheme delimiter found, not an absolute URI
    }
    const std::string scheme = uri.substr(0, schemeEnd);
    if (!isalnum(scheme[0]))
    {
        return false; // Scheme must start with a letter
    }
    // Check if scheme is valid as per RFC2732: letters followed by digits, plus, period, or hyphen
    for (std::string::const_iterator it = scheme.begin(); it != scheme.end(); ++it)
    {
        if (!(isalnum(*it) || *it == '+' || *it == '.' || *it == '-'))
        {
            return false; // Invalid character in scheme, not an absolute URI
        }
    }
    const std::string absPath = schemeEnd + 1 <= uri.size() ? uri.substr(schemeEnd + 1) : "";
    if (!isAbsolutePath(absPath))
    {
        return false; // Absolute path must start with a slash
    }
    return true; // Passed all checks, it is an absolute URI
}
} // namespace utils
