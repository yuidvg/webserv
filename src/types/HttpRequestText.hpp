#pragma once

#include "../webserv.hpp"

typedef Result<const std::string, const int> GetHostNameResult;

class HttpRequestText
{
  private:
    std::string text;
    std::istringstream textStream;
    bool chunked;
    std::string hostName;
    unsigned int clientMaxBodySize;

    void resetTextStream()
    {
        textStream.clear();
        textStream.seekg(0, std::ios::beg);
    };

    bool isChunkedTransfer(const std::string &str)
    {
        return str.find("Transfer-Encoding: chunked") != std::string::npos;
    };

    void readMandatoryText(const std::string &str)
    {
        std::string::size_type pos = str.find("\r\n\r\n");
        if (pos != std::string::npos)
        {
            text = str.substr(0, pos + 4);
            textStream.seekg(pos + 4);
            std::cout << "pos: " << textStream.tellg() << std::endl;
        }
    }

    void readOptionalText()
    {
        if (text.find("POST") != std::string::npos)
        {
            std::string::size_type pos = text.find("\r\n\r\n");
            if (pos != std::string::npos)
            {
                text = text.substr(pos + 4);
                textStream.seekg(pos + 4);
            }
            // posから後ろをtextに格納する
            if (chunked)
            {
                std::string line;
                std::string chunkedText = "";

                while (std::getline(textStream, line))
                {
                    std::istringstream chunkSizeLine(line);
                    int chunkSize;
                    if (!(chunkSizeLine >> std::hex >> chunkSize) || !chunkSizeLine.eof())
                    {
                        break;
                    }
                    if (chunkSize == 0)
                    {
                        break;
                    }

                    std::vector<char> buffer(chunkSize);
                    textStream.read(buffer.data(), chunkSize);
                    chunkedText.append(buffer.data(), chunkSize);

                    // チャンクの末尾のCRLFを読み飛ばす
                    std::getline(textStream, line);
                }
                text += chunkedText;
                chunked = false;
            }
            else
            {
                std::string line;
                std::string chunkedText = "";

                while (std::getline(textStream, line))
                {
                    chunkedText += line;
                }
                text += chunkedText;
            }
        }
    };

  public:
    HttpRequestText(char buffer[]) : hostName(""), clientMaxBodySize(0)
    {
        textStream = std::istringstream(buffer);

        // 入力ストリームから文字列への読み込み
        std::string str((std::istreambuf_iterator<char>(textStream)), std::istreambuf_iterator<char>());
        resetTextStream();

        // chunked トランスファーの判定
        chunked = isChunkedTransfer(str);

        // ヘッダーまでの読み込み
        readMandatoryText(str);
    };

    ~HttpRequestText(){};

    void setClientMaxBodySize(const unsigned int clientMaxBodySize)
    {
        this->clientMaxBodySize = clientMaxBodySize;
    };

    const GetHostNameResult getHostName()
    {
        std::istringstream copyTextStream(text);
        std::string line;

        if (hostName != "")
            return GetHostNameResult::Success(hostName);
        while (std::getline(copyTextStream, line))
        {
            if (line.find("Host: ") != std::string::npos)
            {
                std::string::size_type pos = line.find(": ");
                if (pos != std::string::npos)
                {
                    hostName = line.substr(pos + 2);
                    return GetHostNameResult::Success(hostName);
                }
            }
        }
        return GetHostNameResult::Error(BAD_REQUEST);
    };

    unsigned int getClientMaxBodySize()
    {
        return clientMaxBodySize;
    };

    const std::string getText()
    {
        readOptionalText();
        return text;
    };
};

// HttpRequestText httpRequestText(socket);

// const std::string hostName = httpRequestText.getHostName();
// const Server server = matchedServer(hostName, servers, sd);

// const std::string requestText = httpRequestText.getText();
