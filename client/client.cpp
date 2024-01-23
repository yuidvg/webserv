#include "../src/server/connection.hpp"
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdbool.h>
#include <stdio.h>
#include <string>

#define TRUE 1

bool is_digits_only(const char *str)
{
    while (*str)
    {
        if (!isdigit((unsigned char)*str))
            return false;
        str++;
    }
    return true;
}

int main(int argc, char **argv)
{
    std::string httpRequest;
    if (argc > 2)
    {
        std::ifstream requestFile(argv[2]);
        if (requestFile.is_open())
        {
            std::stringstream buffer;
            buffer << requestFile.rdbuf();
            httpRequest = buffer.str();
            requestFile.close();
        }
        else
        {
            utils::printError(std::string("HTTPリクエストファイルを開けません: " + std::string(argv[2])));
            return -1;
        }
    }
    else if (argc < 2)
    {
        utils::printError("ポート番号を指定してください");
        return -1;
    }
    int sock = 0;
    struct sockaddr_in serv_addr;
    // ソケットの作成
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        utils::printError("Sd creation error");
        return -1;
    }

    serv_addr.sin_family = PF_INET;
    if (!is_digits_only(argv[1]))
    {
        utils::printError("Invalid port number");
        return -1;
    }
    int port = atoi(argv[1]);
    serv_addr.sin_port = htons(port);

    // IPv4アドレスへの変換
    if (inet_pton(PF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        utils::printError("Invalid address / Address not supported");
        return -1;
    }

    // サーバーへの接続
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        utils::printError("Connection failed");
        return -1;
    }

    std::cout << "接続は成功しました" << std::endl;
    std::cout << "接続したfd: " << sock << std::endl;
    usleep(2000000);
    while (TRUE)
    {
        // メッセージの送信
        if (argc > 2)
        {
            send(sock, httpRequest.c_str(), httpRequest.length(), 0);
            std::cout << "HTTPリクエスト送信: " << std::endl << httpRequest << std::endl;
            std::cout << "Message sent" << std::endl;
        }
        else
        {
            std::string message = "Hello Server " + std::string(argv[1]) + " ^_^";
            send(sock, message.c_str(), message.length(), 0);
            std::cout << "Message sent" << std::endl;
        }

        // レスポンスの受信
        char buffer[1024] = {0};
        int valread = read(sock, buffer, 1024);
        if (valread < 0)
        {
            utils::printError(std::string("Read error: " + std::string(strerror(errno))));
            return -1;
        }
        std::cout << "Server: " << buffer << std::endl;
        usleep(3000000);
    }
    // ソケットのクローズ
    close(sock);

    return 0;
}
