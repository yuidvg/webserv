#pragma once
#include "../webserv.hpp"

namespace
{
class CgiResponse
{
  private:
    std::string body;
    std::string contentType;
    std::string location;
    unsigned int statusCode;
    Headers headers;

    void parseStatusLine(const std::string &line)
    {
        const size_t expectedParts = 2;
        std::vector<std::string> statusLine = utils::split(line, " ");
        if (statusLine.size() >= expectedParts)
        {
            statusCode = std::stoi(statusLine[1]);
        }
    }

    void parseHeadersAndBody(const std::vector<std::string> &lines)
    {
        for (size_t i = 1; i < lines.size(); ++i)
        {
            if (lines[i].empty())
            {
                break;
            }
            processHeaderLine(lines[i]);
        }
        for (size_t i = 0; i < lines.size(); ++i)
        {
            if (i > 0)
            {
                body += lines[i] + CRLF;
            }
        }
    }

    void processHeaderLine(const std::string &line)
    {
        std::vector<std::string> header = utils::split(line, ": ");
        if (header.size() > 1)
        {
            if (header[0] == "Content-Type")
                contentType = header[1];
            else if (header[0] == "Location")
                location = header[1];
            else
                headers[header[0]] = header[1];
        }
    }

  public:
    CgiResponse(const std::string &text) : body(""), contentType(""), location(""), statusCode(SUCCESS), headers(Headers())
    {
        std::vector<std::string> lines = utils::split(text, CRLF);
        if (!lines.empty())
            parseStatusLine(lines[0]);
        parseHeadersAndBody(lines);
    }
    const std::string getBody() const
    {
        return body;
    }
    const std::string getContentType() const
    {
        return contentType;
    }
    const std::string getLocation() const
    {
        return location;
    }
    unsigned int getStatusCode() const
    {
        return statusCode;
    }
};
} // namespace
