#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstddef>

#include "base.h"

int create_unix_domain_socket(int type)
{
    if (type != SOCK_STREAM && type != SOCK_DGRAM) {
        perror("invalid type");
        return -1;
    }
    return socket(AF_UNIX, type, 0);
}

void set_sockaddr(struct sockaddr_un *socket_addr, const char *sock_path)
{
    socket_addr->sun_family = AF_UNIX;
    strcpy(socket_addr->sun_path, sock_path);
}

int addr_bind(int socket_fd, struct sockaddr_un *socket_addr)
{
    auto len = sock_len(socket_addr->sun_path);

    return bind(socket_fd, (struct sockaddr *)socket_addr, len);
}

void to_upper(char *s)
{
    size_t len = strlen(s);

    for (size_t i = 0; i != len; ++i) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] -= 32;
        }
    }
}

void unlink_sock(const char *sock_path)
{
    if (0 == access(sock_path, F_OK)) {
        // 文件存在
        unlink(sock_path);
    }
}

bool existent_sock(const char *sock_path)
{
    bool ret = false;

    if (0 == access(sock_path, F_OK | W_OK)) {
        // 文件存在且可写
        ret = true;
    }

    return ret;
}

socklen_t sock_len(const char *sock_path)
{
    return offsetof(struct sockaddr_un, sun_path) + strlen(sock_path);
}
