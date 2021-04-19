/* 服务端：
 * 1. 
 * 2.
 */

#include "lib/common.h"

static int count;   // 连接数

static void sig_int(int signo) {
    printf("\nreceived %d datagrams\n", count);
    exit(0);
}

char *run_cmd(char *cmd) {
    char *data = malloc(16384);
    bzero(data, sizeof(data));
    char *data_index = data;

    FILE *fdp;
    const int max_buffer = 256;
    char buffer[max_buffer];
    fdp = popen(cmd, "r");
    if (fdp) {
        while (!feof(fdp)) {
            if (fgets(buffer, max_buffer, fdp) != NULL) {
                int len = strlen(buffer);
                memcpy(data_index, buffer, len);
                data_index += len;
            }
        }
        pclose(fdp);
    }
    return data;
}

int main(int argc, char **argv) {
    int listenfd;   // 监听socket fd
    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(1);
    }
    
    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERV_PORT);

    int on = 1;
    // 新创建的socket fd，地址复用
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    int rt1 = bind(listenfd, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (rt1 < 0) {
        perror("bind failed");
        exit(1);
    }

    int rt2 = listen(listenfd, LISTENQ);
    if (rt2 < 0) {
        perror("listen failed");
        exit(1);
    }

    signal(SIGPIPE, SIG_IGN);   // 对方发送RST分节后，继续写会产生该信号
    signal(SIGINT, sig_int);

    int connfd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    char buf[256];
    count = 0;

    while (1) {
        // 一次只服务一个客户端
        if ( (connfd = accept(listenfd, (struct sockaddr *) &client_addr, &client_len)) < 0) {
            perror("accept failed");
            exit(1);
        }

        // 处理一个客户端连接的消息
        while (1) {
            bzero(buf, sizeof(buf));    // 清空buf
            int n = read(connfd, buf, sizeof(buf));
            if (n < 0) {
                fprintf(stderr, "error read message\n");
                exit(1);
            } else if (0 == n) {    // 客户端已关闭
                printf("client closed\n");
                close(connfd);
                break;
            }
            count++;    // 连接数+1
            buf[n] = 0;
            if (strncmp(buf, "ls", n) == 0) {
                char *result = run_cmd("ls");
                if (send(connfd, result, strlen(result), 0) < 0) {
                    exit(1);
                }
                free(result);
            } else if (strncmp(buf, "pwd", n) == 0) {
                char buf[256];
                char *result = getcwd(buf, 256);
                if (send(connfd, result, strlen(result), 0) < 0)
                    exit(1);
            } else if (strncmp(buf, "cd ", 3) == 0) {
                char target[256];
                bzero(target, sizeof(target));
                memcpy(target, buf + 3, strlen(buf) -3);
                if (chdir(target) == -1) {
                    fprintf(stderr, "change dir failed, %s\n", target);
                }
            } else {
                char *error = "error: unknown input type";
                if (send(connfd, error, strlen(error), 0) < 0)
                    exit(1);
            }
        }
    }

    exit(0);
}
