// 接続をacceptしてから、recを起動するようにしたい
// serv_sendが接続を受け付けてから、serv_send自身がrecを起動するように変更する
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pthread.h"
// #include "network/network.h"
#include "network/network3.h"
int main(int argc, char *argv[]) {
    signal(SIGPIPE, SIG_IGN);
    int status = 0;
    if (argc == 4) {
        GenClient(argv[1], atoi(argv[2]),atoi(argv[3]),&status);
    }
    if (argc == 3) {
        // GenServer(atoi(argv[1]));
        GenServer(atoi(argv[1]),atoi(argv[2]));
        sleep(3);
        GenClient("0.0.0.0", atoi(argv[1]),atoi(argv[2]),&status);
    }
    while (1) {
        // printf("h\n");
    }
    return 0;
}
