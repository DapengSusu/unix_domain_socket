#include <cstdio>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/un.h>

#include "base.h"

int main()
{
    int socket_fd;
    int ret = 0;
    char buf[BUF_SIZE];
    struct sockaddr_un server_addr, client_addr;

    do {
        // 创建 Unix 域套接字
        socket_fd = create_unix_domain_socket(SOCK_DGRAM);
        if (-1 == socket_fd) {
            perror("create socket fail");
            ret = -1;
            return ret;
        }

        unlink_sock(server_sock_path);

        server_addr.sun_family = AF_UNIX;
        strcpy(server_addr.sun_path, server_sock_path);
        client_addr.sun_family = AF_UNIX;
        strcpy(client_addr.sun_path, client_sock_path);

        // 计算绑定地址长度
        socklen_t server_sock_len = sock_len(server_sock_path);
        socklen_t client_sock_len = sock_len(client_sock_path);

        ret = bind(socket_fd, (struct sockaddr *)&server_addr, server_sock_len);
        if (-1 == ret) {
            perror("bind addr fail");
            break;
        }

        memset(buf, 0, sizeof(buf));
        ssize_t size = recvfrom(
            socket_fd,
            buf,
            sizeof(buf),
            0,
            (struct sockaddr *)&client_addr,
            &client_sock_len
        );
        if (-1 == size) {
            perror("receive msg fail");
            ret = -1;
            break;
        }
        printf("recv: %s\n", buf);

        // 将收到的内容中的字母转换为大写
        to_upper(buf);

        size = sendto(
            socket_fd,
            buf,
            sizeof(buf),
            0,
            (struct sockaddr *)&client_addr,
            client_sock_len
        );
        if (-1 == size) {
            perror("send msg fail");
            ret = -1;
            break;
        }
        printf("send: %s\n", buf);

    } while (0);

    close(socket_fd);    
    unlink_sock(server_sock_path);

    return ret;
}
