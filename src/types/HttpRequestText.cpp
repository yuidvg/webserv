#include "HttpRequestText.hpp"

HttpRequestText::HttpRequestText(char buffer[])
{
    iss.str(buffer);

     // メンバー変数issからホスト名を取得する
    std::istream is(iss.rdbuf());
    std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

    std::string seekStr = "Host: ";
    std::string::size_type pos = str.find(seekStr) + seekStr.length();
    std::string::size_type endPos = str.find("\r\n", pos);
    if (pos != std::string::npos && endPos != std::string::npos)
    {
        hostName = str.substr(pos, endPos - pos);
    }
    else
    {
        hostName = "";
    }
}

HttpRequestText::~HttpRequestText()
{
}

const std::string HttpRequestText::getHostName() const
{
    return hostName;
}

const std::string HttpRequestText::getText() const
{
    if (!chunked)
    {
        return text;
    }
    else
    {
        // chunkedの場合は、chunkedを解除したtextを返す
    }
    return "";
}
