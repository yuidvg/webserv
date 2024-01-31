#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

std::string getUserNameFromDigestAuth(const std::string &authHeader)
{
    const std::string prefix = "Digest username=\"";
    const size_t prefixStartPos = authHeader.find(prefix);
    if (prefixStartPos != std::string::npos)
    {
        const size_t startPos = prefixStartPos + prefix.size();
        const size_t endPos = authHeader.find('\"', startPos);
        if (endPos != std::string::npos)
        {
            return authHeader.substr(startPos, endPos - startPos);
        }
    }
    return "";
}

int main()
{
    // Example Authorization header
    std::string authorizationHeader =
        "Digest username=\"alice123\", realm=\"example\", nonce=\"123abc\", uri=\"/protected\", response=\"456def\"";

    std::string username = getUserNameFromDigestAuth(authorizationHeader);
    std::cout << "Username: " << username << std::endl;

    return 0;
}
