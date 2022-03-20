#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstddef>

// 创建 Unix 域套接字
// type: SOCK_STREAM or SOCK_DGRAM
int create_unix_domain_socket(int type)
{
    if (type != SOCK_STREAM && type != SOCK_DGRAM) {
        perror("invalid type");
        return -1;
    }
    return socket(AF_UNIX, type, 0);
}

int main()
{
    int socket_fd;
    int ret = 0;
    char server_sock_path[] = "server.sock";
    char client_sock_path[] = "./client.sock";
    const char buf[] = "a1b22c333$";
    char recv_buf[1024];
    struct sockaddr_un server_addr, client_addr;

    do {
        socket_fd = create_unix_domain_socket(SOCK_DGRAM);
        if (-1 == socket_fd) {
            perror("create socket fail");
            ret = -1;
            break;
        }

        if (0 == access(client_sock_path, F_OK)) {
            // 文件存在
            unlink(client_sock_path);
        }
        server_addr.sun_family = AF_UNIX;
        memcpy(server_addr.sun_path, server_sock_path, sizeof(server_sock_path));

        client_addr.sun_family = AF_UNIX;
        memcpy(client_addr.sun_path, client_sock_path, sizeof(client_sock_path));

        socklen_t server_sock_len = offsetof(struct sockaddr_un, sun_path) + strlen(server_sock_path);
        socklen_t client_sock_len = offsetof(struct sockaddr_un, sun_path) + strlen(client_sock_path);

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
    if (0 == access(client_sock_path, F_OK)) {
        // 文件存在
        unlink(client_sock_path);
    }

    return ret;
}
