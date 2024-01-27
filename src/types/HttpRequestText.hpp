#pragma once

#include "../webserv.hpp"

typedef Result<const std::string, const std::string> FindStrResult;

class HttpRequestText
{
  private:
    std::string text;
    std::istringstream iss;
    bool chunked;
    std::string hostName;
    unsigned int clientMaxBodySize;

  public:
    static FindStrResult findStr(const std::string &str, const std::string &findStr)
    {
        std::string::size_type pos = str.find(findStr);
        if (pos != std::string::npos)
        {
            std::string::size_type pos2 = str.find("\r\n", pos);
            return FindStrResult::Success(str.substr(pos, pos2 - pos));
        }
        else
        {
            return FindStrResult::Error("");
        }
    };

    HttpRequestText(char buffer[], const Socket &socket)
    {
        iss.str(buffer);

        std::string str((std::istreambuf_iterator<char>(iss)), std::istreambuf_iterator<char>());

        // chunkedかどうかを判定する
        if (str.find("Transfer-Encoding: chunked") != std::string::npos)
        {
            chunked = true;
        }
        else
        {
            chunked = false;
        }

        if (!chunked)
        {
            // issのeofまでをtextに格納する
            std::string line;
            while (std::getline(iss, line))
            {
                text += line;
                text += "\n";
            }
        }
        else
        {
            // 最後の\r\n\r\nまでをtextに格納する
            std::string::size_type pos = str.rfind("\r\n\r\n");
            if (pos != std::string::npos)
            {
                text = str.substr(0, pos + 4);
                // pos分だけissを進める
                iss.seekg(pos + 4);
            }
        }

        clientMaxBodySize = CONFIG.getServer(hostName, socket.port).clientMaxBodySize;
    };

    ~HttpRequestText(){};

    const std::string getHostName()
    {
        // メンバー変数issからホスト名を取得する
        std::istream is(iss.rdbuf());
        std::string str((std::istreambuf_iterator<char>(is)), std::istreambuf_iterator<char>());

        const FindStrResult findHostResult = findStr(str, "Host: ");
        if (!findHostResult.success)
        {
            hostName = findHostResult.error;
        }
        else
        {
            hostName = findHostResult.value;
        }
        return hostName;
    };

    const std::string getText()
    {
        if (!chunked)
        {
            return text;
        }
        else
        {
            // chunkedの場合
            std::string line;
            std::string chunkedText = "";
            while (std::getline(iss, line) && chunkedText.length() < clientMaxBodySize)
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

                char *buffer = new char[chunkSize];
                iss.read(buffer, chunkSize);
                chunkedText.append(buffer, chunkSize);
                delete[] buffer;

                // チャンクの末尾のCRLFを読み飛ばす
                std::getline(iss, line);
            }
            text += chunkedText;
        }
        return text;
    };
};

// HttpRequestText httpRequestText(socket);

// const std::string hostName = httpRequestText.getHostName();
// const Server server = matchedServer(hostName, servers, sd);

// const std::string requestText = httpRequestText.getText();
