#ifndef _STREAM_H
#define _STREAM_H

/*
 * Unix 域套接字 —— 流接口
*/

// 创建套接字
int stream_socket();

// 地址绑定
int stream_bind(int socket_fd, struct sockaddr_un *socket_addr);

#endif /* _STREAM_H */