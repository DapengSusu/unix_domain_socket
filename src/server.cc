#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/unistd.h>
#include <sys/un.h>

// 创建 Unix 域套接字
// type: SOCK_STREAM or SOCK_DGRAM
int create_unix_domain_socket(int type)
{
    if (type != SOCK_STREAM || type != SOCK_DGRAM) {
        perror("invalid type");
        return -1;
    }
    return socket(AF_UNIX, type, 0);
}

int main()
{
    int socket_fd;
    int ret = 0;
    char path[] = "./socket.fd";
    struct sockaddr_un server_addr, client_addr;

    do {
        socket_fd = create_unix_domain_socket(SOCK_DGRAM);
        if (-1 == socket_fd) {
            perror("create socket fail");
            ret = -1;
            break;
        }

        server_addr.sun_family = AF_UNIX;
        memcpy(server_addr.sun_path, path, sizeof(path));

        ret = bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (-1 == ret) {
            perror("bind addr fail");
            return ret;
        }
    } while (0);

    return ret;
}
