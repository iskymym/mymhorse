/* 客户端:
 * 1. 能够指定 IP 地址和端口号;
 * 2. 从标准输入输入命令，等待服务端响应并打印;
 */

#include "lib/common.h"

#define MAXLINE    1024


int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "usage: %s <IPAddress> <port>\n", argv[0]);
        exit(1);
    }
    int port = atoi(argv[2]);
    int socket_fd = tcp_client(argv[1], port);

    char recv_line[MAXLINE], send_line[MAXLINE];
    int n;

    fd_set readmask;
    fd_set allreads;
    FD_ZERO(&allreads);
    FD_SET(0, &allreads);           // 标准输入添加到读集合
    FD_SET(socket_fd, &allreads);   // socket_fd添加到读集合

    while (1) {
        // NOTE: 每次select检测前必须重新赋予读集合
        readmask = allreads;
        int rc = select(socket_fd + 1, &readmask, NULL, NULL, NULL);

        if (rc <= 0) {
            perror("select failed");
            exit(1);
        }

        if (FD_ISSET(socket_fd, &readmask)) {
            n = read(socket_fd, recv_line, MAXLINE);
            if (n < 0) {
                perror("read error");
                exit(1);
            } else if (0 == n) {    // 服务端已关闭连接socket
                printf("server closed connection\n");
                break;
            }
            recv_line[n] = 0;
            fputs(recv_line, stdout);
            fputs("\n", stdout);
        }

        if (FD_ISSET(STDIN_FILENO, &readmask)) {
            if (fgets(send_line, MAXLINE, stdin) != NULL) {
                int i = strlen(send_line);
                if ('\n' == send_line[i - 1]) {
                    send_line[i - 1] = 0;
                }

                if (strncmp(send_line, "quit", strlen(send_line)) == 0) {
                    if (shutdown(socket_fd, 1)) {   // 半关闭
                        perror("shutdown failed");
                        exit(1);
                    }
                } else {
                    ssize_t rt = write(socket_fd, send_line, strlen(send_line));
                    if (rt < 0) {
                        perror("write failed");
                        exit(1);
                    }
                }
            } 
        }
    }
    
    exit(0);
}
