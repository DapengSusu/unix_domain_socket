#ifndef _STREAM_H
#define _STREAM_H

/*
 * Unix 域套接字 —— 流接口
*/

// 当绑定地址时由系统创建，仅用于向客户进程告示套接字名字
// 关闭套接字时该文件不会被自动删除，需确保应用退出时对该文件执行解除链接操作
#include <cstddef>
#include <cstdio>
#include <unistd.h>
const char server_sock_path[] = "server.sock";

// 创建套接字
int stream_socket();

// 等待客户端建立连接
int stream_accept(int socket_fd);

// 发送数据
ssize_t send_all(int connection_fd, const char *send_buf, size_t buf_size, int flags = 0);

// 接收数据
ssize_t recv_all(int connection_fd, char *recv_buf, size_t buf_size, int flags = 0);

#endif /* _STREAM_H */