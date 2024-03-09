#include <fcntl.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
int main(void)
{
    int kev_regist_num; /* 登録するイベント数 */
    struct kevent change;  /* イベント登録・イベント通知取得共用 */
    int listening_socket = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in sin;
    sin.sin_family = PF_INET;
    sin.sin_port = htons(8080);
    sin.sin_addr.s_addr = INADDR_ANY;
    struct sockaddr_in peer_sin;

    struct kevent event;
    const int nonblockSocketFlags = O_NONBLOCK;
    fcntl(listening_socket, F_SETFL, nonblockSocketFlags);
    bind(listening_socket, (struct sockaddr *)&sin, sizeof(sin));
    listen(listening_socket, SOMAXCONN);
    EV_SET(&change, listening_socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    const int kq = kqueue();
    const int registerdNum = kevent(kq, &change, 1, NULL, 0, NULL);
    while (1)
    {
        int n = kevent(kq, NULL, 0, &event, 1, NULL);
        (void)n;
        // kev_regist_num = 0;
        // for (int i = 0; i < n; i++)
        // {
        //     int sock = kev[i].ident;
        //     if (sock == listening_socket)
        //     {
        //         int new_sock = accept(sock, (struct sockaddr *)&peer_sin, &len);
        //         EV_SET(&kev[kev_regist_num++], new_sock, EVFILT_READ, EV_ADD, 0, 0, NULL);
        //     }
        //     else
        //     {
        //         int len = read(sock, buf, sizeof(buf));
        //         if (len == 0)
        //         {
        //             close(sock);
        //         }
        //         else
        //         {
        //             write(sock, buf, len);
        //         }
        //     }
        // }
    }
}
