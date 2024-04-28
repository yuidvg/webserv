#include "../all.hpp"

namespace
{
const std::string CONTENT_TYPE_FIELD = "Content-Type: ";
const std::string LOCATION_FIELD = "Location: ";
const std::string STATUS_FIELD = "Status: ";
bool isContentTypeLine(std::string const &line)
{
    return line.find(CONTENT_TYPE_FIELD) == 0;
}
bool isLocationLine(std::string const &line)
{
    return line.find(LOCATION_FIELD) == 0;
}
bool isStatusLine(std::string const &line)
{
    return line.find(STATUS_FIELD) == 0;
}

HttpRequestResult getCgiHttpRequest(const int cgiSd)
{
    const CgiHttpRequests::iterator it = CGI_HTTP_REQUESTS.find(cgiSd);
    if (it != CGI_HTTP_REQUESTS.end())
    {
        const HttpRequest httpRequest = it->second;
        CGI_HTTP_REQUESTS.erase(it);
        return HttpRequestResult::Success(httpRequest);
    }
    else
    {
        return HttpRequestResult::Error("getHttpRequest: no request for CGI SD " + std::to_string(cgiSd));
    }
}

} // namespace

ParseCgiResponseResult parseCgiResponse(std::string const &response, const int cgiSd)
{
    const std::vector<std::string> headerAndBody = utils::split(response, EMPTY_LINE);
    if (headerAndBody.size() > 0)
    {
        const std::vector<std::string> headerLines = utils::split(headerAndBody[0], "\n");
        // find("Content-Type: ")の後ろにある文字列を取得
        const std::vector<std::string> contentTypeLines = utils::filter(headerLines, isContentTypeLine);
        const std::string contentType =
            contentTypeLines.size() > 0 ? contentTypeLines[0].substr(CONTENT_TYPE_FIELD.length()) : "";
        // find("Location: ")の後ろにある文字列を取得
        const std::vector<std::string> locationLines = utils::filter(headerLines, isLocationLine);
        const std::string location = locationLines.size() > 0 ? locationLines[0].substr(LOCATION_FIELD.length()) : "";
        // find("Status: ")の後ろにある文字列を数字か判定して取得
        const std::vector<std::string> statusLines = utils::filter(headerLines, isStatusLine);
        const StringToIntResult statusResult =
            utils::stringToInt(statusLines.size() > 0 ? statusLines[0].substr(STATUS_FIELD.length()) : "", 100, 599);
        const int status = statusResult.success ? statusResult.value : 0;
        const std::string body =
            headerAndBody.size() > 1 ? response.substr(headerAndBody[0].length() + EMPTY_LINE.length()) : "";
        const HttpRequestResult httpRequestResult = getCgiHttpRequest(cgiSd);
        if (httpRequestResult.success)
        {
            const HttpRequest httpRequest = httpRequestResult.value;
            return ParseCgiResponseResult::Success(
                CgiResponse(cgiSd, httpRequest, contentType, location, status, Headers(), body));
        }
        else
        {
            return ParseCgiResponseResult::Error(httpRequestResult.error);
        }
    }
    else
    {
        return ParseCgiResponseResult::Error("parseCgiResponse: no header in response");
    }
}

CgiResponses parseCgiResponses(const EventDatas &cgiResponseEventDatas)
{
    for (EventDatas::const_iterator it = cgiResponseEventDatas.begin(); it != cgiResponseEventDatas.end(); ++it)
    {
        const EventData &eventData = *it;
        const int cgiSd = eventData.sd;
        const ParseCgiResponseResult parseResult = parseCgiResponse(eventData.data, cgiSd);
    }
}
