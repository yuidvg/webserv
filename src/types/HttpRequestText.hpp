#pragma once

#include "../webserv.hpp"

typedef Result<const std::string, const std::string> FindStrResult;

class HttpRequestText
{
  private:
    std::string text;
    std::istringstream textStream;
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
        textStream = std::istringstream(buffer);

        std::string str((std::istreambuf_iterator<char>(textStream)), std::istreambuf_iterator<char>());
        textStream.clear();
        textStream.seekg(0, std::ios::beg);

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

            while (std::getline(textStream, line)) //  && text.length() < clientMaxBodySize
            {
                line += "\n";
                text += line;
            }

            std::cout << "end\ntext:\n" << text << std::endl;
        }
        else
        {
            // Header後の\r\n\r\nまでをtextに格納する
            std::string::size_type pos = str.rfind("\r\n\r\n");
            if (pos != std::string::npos)
            {
                text = str.substr(0, pos + 4);
                // pos分だけissを進める
                textStream.seekg(pos + 4);
            }
        }

        clientMaxBodySize = CONFIG.getServer(hostName, socket.port).clientMaxBodySize;
    };

    ~HttpRequestText(){};

    const std::string getHostName()
    {
        // メンバー変数issからホスト名を取得する
        std::istream is(textStream.rdbuf());
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
            std::cout << "HERE" << std::endl;
            return text;
        }
        // chunkedの場合
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

            char *buffer = new char[chunkSize];
            textStream.read(buffer, chunkSize);
            chunkedText.append(buffer, chunkSize);
            delete[] buffer;

            // チャンクの末尾のCRLFを読み飛ばす
            std::getline(textStream, line);
        }
        text += chunkedText;
        chunked = false;
        return text;
    };
    std::string readLine()
    {
        std::string line;
        std::istringstream iss(text);

        std::getline(iss, line);

        text.erase(0, line.length() + 1); // 一行分を削除する
        if (line[line.length() - 1] == '\r')
            line.erase(line.end() - 1);                    // 後ろの\rを削除する
        std::replace(line.begin(), line.end(), '\r', ' '); // line中の\rをspaceに置換する
        return line;
    };
    bool eof()
    {
        return text.empty();
    };
    void clear()
    {
        textStream.clear();
        textStream.seekg(0, std::ios::beg);
    };
    unsigned int getClientMaxBodySize()
    {
        return clientMaxBodySize;
    };
};

// HttpRequestText httpRequestText(socket);

// const std::string hostName = httpRequestText.getHostName();
// const Server server = matchedServer(hostName, servers, sd);

// const std::string requestText = httpRequestText.getText();
