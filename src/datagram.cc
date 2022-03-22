#include <cstddef>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>

#include "datagram.h"
#include "base.h"

int datagram_socket()
{
    return create_unix_domain_socket(SOCK_DGRAM);
}

ssize_t datagram_recvfrom(int socket_fd, char *recv_buf, size_t buf_size, struct sockaddr_un *from_socket_addr)
{
    auto len = sock_len(from_socket_addr->sun_path);

    memset(recv_buf, 0, buf_size);
    return recvfrom(
        socket_fd,
        recv_buf,
        buf_size,
        0,
        (struct sockaddr *)from_socket_addr,
        &len
    );
}

ssize_t datagram_sendto(int socket_fd, const char *send_buf, size_t buf_size, struct sockaddr_un *to_socket_addr)
{
    auto len = sock_len(to_socket_addr->sun_path);

    return sendto(
        socket_fd,
        send_buf,
        buf_size,
        0,
        (struct sockaddr *)to_socket_addr,
        len
    );
}
