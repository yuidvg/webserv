#include "../all.hpp"

void processHttpMessageFromCgi(const HttpMessage &httpMessage)
{
    if (httpMessage.tag == LEFT)
    {
        HTTP_REQUESTS.push(httpMessage.leftValue);
    }
    else
    {
        const HttpResponse httpResponse = httpMessage.rightValue;
        if (!appendOutbound(httpResponse.destinationSd, stringify(httpResponse)))
        {
            std::cerr << "Failed to set outbound for socket " << httpResponse.destinationSd << std::endl;
        }
    }
}
