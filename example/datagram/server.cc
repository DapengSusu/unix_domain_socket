#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/un.h>

#include "base.h"
#include "datagram.h"

int main()
{
    int socket_fd;
    int ret = 0;
    char buf[BUF_SIZE];
    struct sockaddr_un server_addr, client_addr;

    do {
        // 创建 Unix 域套接字
        socket_fd = datagram_socket();
        if (-1 == socket_fd) {
            perror("create socket fail");
            ret = -1;
            return ret;
        }

        unlink_sock(server_sock_path);

        // 填充地址结构体
        set_sockaddr(&server_addr, server_sock_path);
        set_sockaddr(&client_addr, client_sock_path);

        // 地址绑定
        ret = addr_bind(socket_fd, &server_addr);
        if (-1 == ret) {
            perror("bind addr fail");
            break;
        }

        // 接收客户端发送
        // 阻塞
        ssize_t size = datagram_recvfrom(socket_fd, buf, sizeof(buf), &client_addr);
        if (-1 == size) {
            perror("receive msg fail");
            ret = -1;
            break;
        }
        printf("recv: %s\n", buf);

        // 将收到的内容中的字母转换为大写
        to_upper(buf);

        // 处理后再发送给客户端
        size = datagram_sendto(socket_fd, buf, sizeof(buf), &client_addr);
        if (-1 == size) {
            perror("send msg fail");
            ret = -1;
            break;
        }
        printf("send: %s, size: %ld\n", buf, size);

    } while (false);

    close(socket_fd);    
    unlink_sock(server_sock_path);

    return ret;
}
