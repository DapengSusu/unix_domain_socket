#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstddef>

#include "base.h"

int main()
{
    int socket_fd;
    int ret = 0;
    const char buf[] = "a1b22c333$";
    char recv_buf[BUF_SIZE];
    struct sockaddr_un server_addr, client_addr;

    do {
        socket_fd = create_unix_domain_socket(SOCK_DGRAM);
        if (-1 == socket_fd) {
            perror("create socket fail");
            ret = -1;
            break;
        }

        unlink_sock(client_sock_path);

        server_addr.sun_family = AF_UNIX;
        strcpy(server_addr.sun_path, server_sock_path);
        client_addr.sun_family = AF_UNIX;
        strcpy(client_addr.sun_path, client_sock_path);

        // 计算绑定地址长度
        socklen_t server_sock_len = sock_len(server_sock_path);
        socklen_t client_sock_len = sock_len(client_sock_path);

        ret = bind(socket_fd, (struct sockaddr *)&client_addr, client_sock_len);
        if (-1 == ret) {
            perror("bind addr fail");
            break;
        }

        ssize_t size = sendto(
            socket_fd,
            buf,
            sizeof(buf),
            0,
            (struct sockaddr *)&server_addr,
            server_sock_len
        );
        if (-1 == size) {
            perror("send msg fail");
            ret = -1;
            break;
        }
        printf("send: %s\n", buf);

        size = recvfrom(
            socket_fd,
            recv_buf,
            sizeof(recv_buf),
            0,
            (struct sockaddr *)&server_addr,
            &server_sock_len
        );
        if (-1 == size) {
            perror("receive msg fail");
            ret = -1;
            break;
        }
        printf("recv: %s\n", recv_buf);

    } while (0);

    close(socket_fd);
    unlink_sock(client_sock_path);

    return ret;
}
