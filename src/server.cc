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

// 字符串转大写
void to_upper(char *s)
{
    size_t len = strlen(s);

    for (size_t i = 0; i != len; ++i) {
        if (s[i] >= 'a' && s[i] <= 'z') {
            s[i] -= 32;
        }
    }
}

int main()
{
    int socket_fd;
    int ret = 0;
    char server_sock_path[] = "server.sock";
    char client_sock_path[] = "./client.sock";
    char buf[1024];
    struct sockaddr_un server_addr, client_addr;

    do {
        socket_fd = create_unix_domain_socket(SOCK_DGRAM);
        if (-1 == socket_fd) {
            perror("create socket fail");
            ret = -1;
            return ret;
        }

        if (0 == access(server_sock_path, F_OK)) {
            // 文件存在
            unlink(server_sock_path);
        }
        server_addr.sun_family = AF_UNIX;
        memcpy(server_addr.sun_path, server_sock_path, strlen(server_sock_path));

        client_addr.sun_family = AF_UNIX;
        memcpy(client_addr.sun_path, client_sock_path, sizeof(client_sock_path));

        // 计算绑定地址长度
        // 先计算 sun_path 成员在 sockaddr_un 结构中的偏移量，再加上路径名长度
        socklen_t server_sock_len = offsetof(struct sockaddr_un, sun_path) + strlen(server_sock_path);
        socklen_t client_sock_len = offsetof(struct sockaddr_un, sun_path) + strlen(client_sock_path);

        ret = bind(socket_fd, (struct sockaddr *)&server_addr, server_sock_len);
        if (-1 == ret) {
            perror("bind addr fail");
            break;
        }

        memset(buf, 0, sizeof(buf));
        ssize_t size = recvfrom(
            socket_fd,
            buf,
            sizeof(buf),
            0,
            (struct sockaddr *)&client_addr,
            &client_sock_len
        );
        if (-1 == size) {
            perror("receive msg fail");
            ret = -1;
            break;
        }
        printf("recv: %s\n", buf);

        // 将收到的内容中的字母转换为大写
        to_upper(buf);

        size = sendto(
            socket_fd,
            buf,
            sizeof(buf),
            0,
            (struct sockaddr *)&client_addr,
            client_sock_len
        );
        if (-1 == size) {
            perror("send msg fail");
            ret = -1;
            break;
        }
        printf("send: %s\n", buf);

    } while (0);

    close(socket_fd);    
    if (0 == access(server_sock_path, F_OK)) {
        // 文件存在
        unlink(server_sock_path);
    }

    return ret;
}
