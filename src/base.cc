#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/unistd.h>
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

socklen_t sock_len(const char *sock_path)
{
    return offsetof(struct sockaddr_un, sun_path) + strlen(sock_path);
}
