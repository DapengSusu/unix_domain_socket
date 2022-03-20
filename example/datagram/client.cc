#include <cstdio>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/un.h>

#include "base.h"
#include "datagram.h"

int main()
{
    int socket_fd;
    int ret = 0;
    const char send_buf[] = "a1b22c333$";
    char recv_buf[BUF_SIZE];
    struct sockaddr_un server_addr, client_addr;

    do {
        // 创建 Unix 域套接字
        socket_fd = datagram_socket();
        if (-1 == socket_fd) {
            perror("create socket fail");
            ret = -1;
            break;
        }

        unlink_sock(client_sock_path);

        // 填充地址结构体
        set_sockaddr(&server_addr, server_sock_path);
        set_sockaddr(&client_addr, client_sock_path);

        // 地址绑定
        ret = datagram_bind(socket_fd, &client_addr);
        if (-1 == ret) {
            perror("bind addr fail");
            break;
        }

        // 发送数据
        auto size = datagram_sendto(socket_fd, send_buf, sizeof(send_buf), &server_addr);
        if (-1 == size) {
            perror("send msg fail");
            ret = -1;
            break;
        }
        printf("send: %s\n", send_buf);

        // 接收处理结果
        size = datagram_recvfrom(socket_fd, recv_buf, sizeof(recv_buf), &server_addr);
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
