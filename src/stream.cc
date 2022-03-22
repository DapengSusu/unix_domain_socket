#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

#include "base.h"
#include "stream.h"

int stream_socket()
{
    return create_unix_domain_socket(SOCK_STREAM);
}

int stream_accept(int socket_fd)
{
    return accept(socket_fd, (struct sockaddr *)NULL, (socklen_t *)NULL);
}

ssize_t send_all(int connection_fd, const char *send_buf, size_t buf_size, int flags)
{
    ssize_t size = send(connection_fd, send_buf, buf_size, flags);

    return size;
}

ssize_t recv_all(int connection_fd, char *recv_buf, size_t buf_size, int flags)
{
    memset(recv_buf, 0, buf_size);
    ssize_t size = recv(connection_fd, recv_buf, buf_size, flags);

    return size;
}
