#include "common.h"

int tcp_server(int port) {
    int listenfd;
    if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(1);
    }

    struct sockaddr_in server_addr;
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // 对新创建的socket fd设置地址复用
    int on = 1;
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

    signal(SIGPIPE, SIG_IGN);   // avoid stopping process

    int connfd;
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    if ( (connfd = accept(listenfd, (struct sockaddr *) &client_addr, &client_len)) < 0) {
        perror("accept failed");
        exit(1);
    }

    return connfd;
}
