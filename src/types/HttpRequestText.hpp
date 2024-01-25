#include "../webserv.hpp"

class HttpRequestText
{
  private:
    std::string text;
    std::istringstream iss;
    bool chunked;
    std::string hostName;

  public:
    HttpRequestText();
    ~HttpRequestText();
};

HttpRequestText::HttpRequestText(/* args */)
{
}

HttpRequestText::~HttpRequestText()
{
}

HttpRequestText httpRequestText(socket);

const std::string hostName = httpRequestText.getHostName();
const Server server = matchedServer(hostName, servers, sd);

const std::string requestText = httpRequestText.getText();
