#include ".hpp"

bool isLineTooLong(const std::string &line)
{
    if (line.length() > MAX_LEN)
        return true;
    else
        return false;
}

int getRequestLineStatusCode(const RequestLine requestLine)
{
    if (requestLine.target.find(':') != std::string::npos &&
        requestLine.target.find('*') != std::string::npos) // CONNECT, OPTIONSは非対応
        return BAD_REQUEST;

    if (requestLine.version != SERVER_PROTOCOL)
        return BAD_REQUEST;

    return SUCCESS;
}

bool validateAndDecodeTarget(std::string &target)
{
    std::string decodedTarget;
    std::string::iterator it = target.begin();

    while (it != target.end())
    {
        if (*it == '%')
        {
            if (it + 2 >= target.end() || !std::isxdigit(*(it + 1)) || !std::isxdigit(*(it + 2)))
                return false;
            decodedTarget += utils::hexToUtf8Char(target.substr(it - target.begin() + 1, 2));
            it += 3;
        }
        else
        {
            decodedTarget += *it;
            it++;
        }
    }
    target = decodedTarget;
    return true;
}

GetRequestLineResult getRequestLine(std::istringstream &requestTextStream)
{
    std::string method, target, version;
    std::string line;

    while ((line = utils::getlineCustom(requestTextStream)).empty())
        ;

    if (!requestTextStream.eof() && !isLineTooLong(line))
    {
        std::istringstream requestLine(line);
        if ((requestLine >> method >> target >> version) && requestLine.eof() && validateAndDecodeTarget(target))
        {
            const RequestLine requestLineElements = {method, target, version};
            return GetRequestLineResult::Success(requestLineElements);
        }
        else
            return GetRequestLineResult::Error(BAD_REQUEST);
    }
    else
        return GetRequestLineResult::Error(BAD_REQUEST);
}
