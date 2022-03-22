#include <cstdio>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/un.h>

#include "base.h"
#include "stream.h"

int main()
{
    int connection_fd;
    int ret = 0;
    const char send_buf[] = "a1bb2ccc3#";
    char recv_buf[BUF_SIZE];
    struct sockaddr_un client_addr;

    do {
        // 创建 Unix 域套接字
        connection_fd = stream_socket();
        if (-1 == connection_fd) {
            perror("create socket fail");
            ret = -1;
            return ret;
        }

        // 确保 sock 文件存在且可写
        if (false == existent_sock(server_sock_path)) {
            perror("nonexistent sock file");
            ret = -1;
            break;
        }

        // 填充地址结构体
        set_sockaddr(&client_addr, server_sock_path);

        // 连接服务端
        ret = connect(connection_fd, (struct sockaddr *)&client_addr, sock_len(server_sock_path));
        if (-1 == ret) {
            perror("connect server fail");
            break;
        }

        // 发送数据给服务端
        ssize_t size = send_all(connection_fd, send_buf, sizeof(send_buf));
        if (-1 == size) {
            perror("send data fail");
            break;
        }
        printf("send: %s, size: %ld\n", send_buf, size);

        // 接收服务端的处理结果
        size = recv_all(connection_fd, recv_buf, BUF_SIZE);
        if (-1 == size) {
            perror("recv data fail");
            break;
        }
        printf("recv: %s, size: %ld\n", recv_buf, size);
    } while (false);

    close(connection_fd);

    return ret;
}
