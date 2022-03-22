#ifndef _BASE_H
#define _BASE_H

#include <sys/socket.h>

#define BUF_SIZE 1024

// 创建 Unix 域套接字
// type: SOCK_STREAM or SOCK_DGRAM
int create_unix_domain_socket(int type);

// 填充套接字地址结构体
void set_sockaddr(struct sockaddr_un *socket_addr, const char *sock_path);

// 地址绑定
int addr_bind(int socket_fd, struct sockaddr_un *socket_addr);

// 字符串转大写
void to_upper(char *s);

// 删除 sock 文件
void unlink_sock(const char *sock_path);

// 是否存在有效的 sock 文件
bool existent_sock(const char *sock_path);

// 计算绑定地址长度
// 先计算 sun_path 成员在 sockaddr_un 结构中的偏移量，再加上路径名长度
socklen_t sock_len(const char *sock_path);

#endif /* _BASE_H */