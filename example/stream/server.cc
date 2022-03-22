#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/un.h>

#include "base.h"
#include "stream.h"

int main()
{
    int socket_fd, connection_fd;
    int ret = 0;
    char buf[BUF_SIZE];
    struct sockaddr_un server_addr;

    do {
        // 创建 Unix 域套接字
        socket_fd = stream_socket();
        if (-1 == socket_fd) {
            perror("create socket fail");
            ret = -1;
            return ret;
        }

        unlink_sock(server_sock_path);

        // 填充地址结构体
        set_sockaddr(&server_addr, server_sock_path);

        // 地址绑定
        ret = addr_bind(socket_fd, &server_addr);
        if (-1 == ret) {
            perror("bind addr fail");
            break;
        }

        // listen 函数的调用把主动套接字变为被动套接字（作为服务端）
        ret = listen(socket_fd, 5);
        if (-1 == ret) {
            perror("listen fail");
            break;
        }

        // 等待客户端建立连接（阻塞）
        connection_fd = stream_accept(socket_fd);
        if (-1 == connection_fd) {
            perror("accept connection fail");
            break;
        }

        // 接收客户端数据
        ssize_t size = recv_all(connection_fd, buf, BUF_SIZE);
        if (-1 == size) {
            perror("recv data fail");
            break;
        }
        printf("recv: %s, size: %ld\n", buf, size);

        // 字母转为大写
        to_upper(buf);

        // 处理后的字符串发送给客户端
        size = send_all(connection_fd, buf, strlen(buf) + 1);
        if (-1 == size) {
            perror("send data fail");
            break;
        }
        printf("send: %s, size: %ld\n", buf, size);

        close(connection_fd);
    } while (false);

    close(socket_fd);
    unlink_sock(server_sock_path);

    return ret;
}
