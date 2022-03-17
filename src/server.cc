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
    char buf[1024];
    struct sockaddr_un server_addr, client_addr;

    do {
        socket_fd = create_unix_domain_socket(SOCK_DGRAM);
        if (-1 == socket_fd) {
            perror("create socket fail");
            ret = -1;
            break;
        }

        if (0 == access(server_fd_path, F_OK)) {
            // 文件存在
            unlink(server_fd_path);
        }
        server_addr.sun_family = AF_UNIX;
        memcpy(server_addr.sun_path, server_fd_path, sizeof(server_fd_path));

        ret = bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (-1 == ret) {
            perror("bind addr fail");
            break;
        }

        while (true) {
            memset(buf, 0, sizeof(buf));
            ssize_t size = recvfrom(
                socket_fd, buf,
                sizeof(buf),
                0,
                (struct sockaddr *)&client_addr,
                (socklen_t *)sizeof(client_addr)
            );
            if (-1 == size) {
                perror("receive msg fail");
                ret = -1;
                break;
            }

            printf("recv: %s\n", buf);
        }
        
    } while (0);

    close(socket_fd);

    return ret;
}
