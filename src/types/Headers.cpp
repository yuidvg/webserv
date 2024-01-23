#include "Headers.hpp"

Headers::Headers()
{
}
Headers::Headers(const std::string &contentType, const std::string &contentLength, const std::string &location,
                 const std::string &allow)
{
    Headers headers;
    headers["Content-Type"] = contentType;
    headers["Content-Length"] = contentLength;
    headers["Location"] = location;
    *this = headers;
}
