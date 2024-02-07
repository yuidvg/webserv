#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

namespace
{
const std::string BASE_64_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "abcdefghijklmnopqrstuvwxyz"
                                  "0123456789+/";
bool isBase64(unsigned char c)
{
    return std::find(BASE_64_CHARS.begin(), BASE_64_CHARS.end(), c) != BASE_64_CHARS.end();
}

// Function to decode a block of 4 base64 characters into 3 bytes
std::string decodeBlock(const char *encoded)
{
    char base64Chars[4], decodedChars[3];
    std::string ret;
    for (int i = 0; i < 4; i++)
        base64Chars[i] = encoded[i] == '=' ? 0 : BASE_64_CHARS.find(encoded[i]);

    decodedChars[0] = (base64Chars[0] << 2) + ((base64Chars[1] & 0x30) >> 4);
    decodedChars[1] = ((base64Chars[1] & 0xf) << 4) + ((base64Chars[2] & 0x3c) >> 2);
    decodedChars[2] = ((base64Chars[2] & 0x3) << 6) + base64Chars[3];

    for (int i = 0; (i < 3); i++)
        ret += decodedChars[i];
    return ret;
}

std::string decodeBase64(const std::string &base64String)
{
    std::string out;
    int i = 0, j = 0;
    int in_len = base64String.length();
    char blockBuf[4];

    while (in_len-- && (base64String[i] != '=') && isBase64(base64String[i]))
    {
        blockBuf[j++] = base64String[i];
        i++;
        if (j == 4)
        {
            out += decodeBlock(blockBuf);
            j = 0;
        }
    }

    if (j)
    {
        for (int k = j; k < 4; k++)
            blockBuf[k] = 0;

        out += decodeBlock(blockBuf);
    }

    return out;
}

std::string getUserNameFromBasicAuth(const std::string &authHeader)
{
    std::string prefix = "Basic ";
    if (authHeader.substr(0, prefix.size()) == prefix)
    {
        std::string encoded = authHeader.substr(prefix.size());
        std::string decoded = decodeBase64(encoded);
        size_t colonPos = decoded.find(':');
        if (colonPos != std::string::npos)
        {
            return decoded.substr(0, colonPos);
        }
    }
    return "";
}
} // namespace

int main()
{
    // Example Authorization header
    std::string authorizationHeader = "Basic YWxpY2UxMjM6cGFzc3dvcmQxMjM=";

    std::string username = getUserNameFromBasicAuth(authorizationHeader);
    std::cout << "Username: " << username << std::endl;

    return 0;
}
