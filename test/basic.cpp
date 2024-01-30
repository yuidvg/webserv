#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

std::string base64_decode(const std::string &in)
{
    // Implement Base64 decoding function here
    const std::string base64_chars =
                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                 "abcdefghijklmnopqrstuvwxyz"
                 "0123456789+/";

    // Function to check if a character is a valid base64 character
    bool is_base64(unsigned char c) {
        return (isalnum(c) || (c == '+') || (c == '/'));
    }

    // Function to decode a block of 4 base64 characters into 3 bytes
    std::string decode_block(const char* encoded) {
        char char_array_4[4], char_array_3[3];
        std::string ret;
        for (int i = 0; i < 4; i++)
            char_array_4[i] = encoded[i] == '=' ? 0 : base64_chars.find(encoded[i]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (int i = 0; (i < 3); i++)
            ret += char_array_3[i];
        return ret;
    }

    std::string out;
    int i = 0, j = 0;
    int in_len = in.length();
    char char_array_4[4];

    while (in_len-- && (in[i] != '=') && is_base64(in[i])) {
        char_array_4[j++] = in[i]; i++;
        if (j == 4) {
            out += decode_block(char_array_4);
            j = 0;
        }
    }

    if (j) {
        for (int k = j; k < 4; k++)
            char_array_4[k] = 0;

        out += decode_block(char_array_4);
    }

    return out;
    return "alice123:password123";
}

std::string getUserNameFromBasicAuth(const std::string &authHeader)
{
    std::string prefix = "Basic ";
    if (authHeader.substr(0, prefix.size()) == prefix)
    {
        std::string encoded = authHeader.substr(prefix.size());
        std::string decoded = base64_decode(encoded);
        size_t colonPos = decoded.find(':');
        if (colonPos != std::string::npos)
        {
            return decoded.substr(0, colonPos);
        }
    }
    return "";
}

int main()
{
    // Example Authorization header
    std::string authorizationHeader = "Basic YWxpY2UxMjM6cGFzc3dvcmQxMjM=";

    std::string username = getUserNameFromBasicAuth(authorizationHeader);
    std::cout << "Username: " << username << std::endl;

    return 0;
}
