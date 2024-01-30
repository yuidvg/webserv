#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

std::string getUserNameFromDigestAuth(const std::string &authHeader)
{
    std::string prefix = "Digest username=\"";
    size_t startPos = authHeader.find(prefix);
    if (startPos != std::string::npos)
    {
        startPos += prefix.size();
        size_t endPos = authHeader.find('\"', startPos);
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
