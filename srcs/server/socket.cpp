#include "socket.hpp"

int server(void)
{
    int i, len, rc;
    int listen_sd;
    int max_sd;
    int new_sd = 0;
    int desc_ready, end_server = FALSE;
    int close_conn;
    char buffer[80];
    struct sockaddr_in6 addr;
    struct timeval timeout;
    struct fd_set master_set, working_set;
    listen_sd = socket(AF_INET6, SOCK_STREAM, 0);
    if (listen_sd < 0)
    {
        std::cout << RED << "socket() failed: " << strerror(errno) << NORMAL << std::endl;
        exit(-1);
    }

    int flags = fcntl(listen_sd, F_GETFL, 0);
    if (flags < 0)
    {
        std::cout << RED << "fcntl() failed: " << strerror(errno) << NORMAL << std::endl;
        close(listen_sd);
        exit(-1);
    }
    flags |= O_NONBLOCK;
    rc = fcntl(listen_sd, F_SETFL, flags);
    if (rc < 0)
    {
        std::cout << RED << "fcntl() failed: " << strerror(errno) << NORMAL << std::endl;
        close(listen_sd);
        exit(-1);
    }

    addr.sin6_family = AF_INET6;
    addr.sin6_addr = in6addr_any;
    addr.sin6_port = htons(SERVER_PORT);
    rc = bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr));
    if (rc < 0)
    {
        std::cout << RED << "bind() failed" << strerror(errno) << NORMAL << std::endl;
        close(listen_sd);
        exit(-1);
    }
    rc = listen(listen_sd, 5);
    if (rc < 0)
    {
        std::cout << RED << "listen() failed" << strerror(errno) << NORMAL << std::endl;
        close(listen_sd);
        exit(-1);
    }
    FD_ZERO(&master_set);
    max_sd = listen_sd;
    /*listen_sdをmaster_setに追加します。 */
    // listen_sd = 常に新しい接続要求を受け入れる(リスニングソケット)
    FD_SET(listen_sd, &master_set);

    timeout.tv_sec = 3 * 60;
    timeout.tv_usec = 0;
    while (end_server == FALSE)
    {
        memcpy(&working_set, &master_set, sizeof(master_set));
        std::cout << "Waiting on select()... " << std::endl;
        rc = select(max_sd + 1, &working_set, NULL, NULL, &timeout);

        if (rc < 0)
        {
            std::cout << RED << "select() failed: " << strerror(errno) << NORMAL << std::endl;
            break;
        }
        else if (rc == 0)
        {
            std::cout << "select() timed out. End program.\n";
            break;
        }
        desc_ready = rc;
        for (i = 0; i <= max_sd && desc_ready > 0; ++i)
        {
            /* FD_ISSET()は、指定したファイルディスクリプタがセット内に存在するかどうかをチェックします。
             */
            if (FD_ISSET(i, &working_set))
            {
                desc_ready -= 1;
                // 新しい接続要求を処理する
                if (i == listen_sd)
                {
                    std::cout << "  Listening socket is readable" << std::endl;
                    while (new_sd != -1)
                    {
                        new_sd = accept(listen_sd, NULL, NULL);
                        std::cout << RED << "new_sd = " << new_sd << NORMAL << std::endl;
                        if (new_sd < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                std::cout << RED << "  accept() failed" << strerror(errno) << NORMAL << std::endl;
                                end_server = TRUE;
                            }
                            break;
                        }

                        std::cout << "  New incoming connection " << new_sd << std::endl;
                        FD_SET(new_sd, &master_set);
                        if (new_sd > max_sd)
                            max_sd = new_sd;
                    }
                    new_sd = 0;
                }
                else // 接続済みのソケットと通信する
                {
                    std::cout << "  Descriptor " << i << "is readable" << std::endl;
                    close_conn = FALSE;
                    while (TRUE)
                    {
                        rc = recv(i, buffer, sizeof(buffer), 0);
                        std::cout << buffer << std::endl;
                        if (rc < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                std::cout << RED << "  recv() failed" << strerror(errno) << NORMAL << std::endl;
                                close_conn = TRUE;
                            }
                            break;
                        }

                        if (rc == 0)
                        {
                            std::cout << "Connection closed" << std::endl;
                            close_conn = TRUE;
                            break;
                        }
                        len = rc;
                        std::cout << len << " bytes received" << std::endl;
                        rc = send(i, "Yeah Client", 12, 0);
                        if (rc < 0)
                        {
                            std::cout << RED << "  send() failed" << strerror(errno) << NORMAL << std::endl;
                            close_conn = TRUE;
                            break;
                        }
                    }

                    if (close_conn == TRUE)
                    {
                        close(i);
                        FD_CLR(i, &master_set);
                        if (i == max_sd)
                        {
                            while (FD_ISSET(max_sd, &master_set) == FALSE)
                                max_sd -= 1;
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i <= max_sd; ++i)
    {
        if (FD_ISSET(i, &master_set))
            close(i);
    }
    return 0;
}
