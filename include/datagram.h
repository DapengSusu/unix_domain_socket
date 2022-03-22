#ifndef _DATAGRAM_H
#define _DATAGRAM_H

/*
 * Unix 域套接字 —— 数据报接口
*/

#include "base.h"

// 当绑定地址时由系统创建，仅用于向客户进程告示套接字名字
// 关闭套接字时该文件不会被自动删除，需确保应用退出时对该文件执行解除链接操作
const char server_sock_path[] = "server.sock";
const char client_sock_path[] = "client.sock";

// 创建套接字
int datagram_socket();

// 地址绑定
int datagram_bind(int socket_fd, struct sockaddr_un *socket_addr);

// 接收数据
ssize_t datagram_recvfrom(int socket_fd, char *recv_buf, size_t buf_size, struct sockaddr_un *from_socket_addr);

// 发送数据
ssize_t datagram_sendto(int socket_fd, const char *send_buf, size_t buf_size, struct sockaddr_un *to_socket_addr);

#endif /* _DATAGRAM_H */
