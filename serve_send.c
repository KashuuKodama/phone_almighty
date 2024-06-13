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
#include "network/network.h"
int main(int argc, char *argv[]) {
    if (argc == 3) {
        GenClient(argv[1],atoi(argv[2]));
    }
    if (argc == 2) {
        GenServer(atoi(argv[1]));
    }
    while (1) {
    }
    return 0;
}
