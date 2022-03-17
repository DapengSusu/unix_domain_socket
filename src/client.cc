#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/un.h>
#include <unistd.h>

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
    char server_fd_path[] = "./server.fd";
    char client_fd_path[] = "./client.fd";
    const char buf[] = "abc";
    struct sockaddr_un server_addr, client_addr;

    do {
        socket_fd = create_unix_domain_socket(SOCK_DGRAM);
        if (-1 == socket_fd) {
            perror("create socket fail");
            ret = -1;
            break;
        }

        // ret = access(path, F_OK);
        // if (-1 == ret) {
        //     // 文件不存在
        //     perror("fd file no-exist");
        //     break;
        // }

        server_addr.sun_family = AF_UNIX;
        memcpy(server_addr.sun_path, server_fd_path, sizeof(server_fd_path));

        client_addr.sun_family = AF_UNIX;
        memcpy(client_addr.sun_path, client_fd_path, sizeof(client_fd_path));

        ret = bind(socket_fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
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
            (socklen_t)sizeof(server_addr)
        );
        if (-1 == size) {
            perror("send msg fail");
            ret = -1;
            break;
        }
    } while (0);

    close(socket_fd);

    return ret;
}
