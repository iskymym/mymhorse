#ifndef MYMHORSE_LIB_COMMON_H
#define MYMHORSE_LIB_COMMON_H

#include <sys/socket.h>     // basic socket definitions
#include <netinet/in.h>     // sockaddr_in{} and other Internet defns
#include <arpa/inet.h>      // inet(3) functions

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/select.h>
#include <strings.h>        // for bzero()


int tcp_server(int port);
int tcp_client(char *address, int port);

#define SERV_PORT    10087
#define LISTENQ      1024

#endif // MYMHORSE_LIB_COMMON_H
