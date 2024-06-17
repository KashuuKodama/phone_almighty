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
#include "filedata.h"
#include "complex.h"
#include "../fft.h"
#define N SampleSize*4000/40000
typedef struct 
{
    char ip[256];
    int port;
}ThreadInfo;

/*  サーバーを立ててクライアントとやりとりする
    -   (今はやらない)サーバーのポート番号は、指定された個数だけ、htons(0)によりランダムに割り当て、getsocknameで取得して、(共通の)配列に格納する
    -   複数のクライアントと同時にやりとりできるように、各クライアント用の個別のスレッドを立てる
    -   全てのクライアントから受け取ったスペクトルデータを線形結合して再生するとともに、録音データのスペクトルと線形結合して全てのクライアントに送信する
    -   線形結合を取るために、共通のスペクトル用の配列を用意し、mutexを用いて排他制御しつつ、並列に上書きしていく
 */
void* server_thread(void* param){
    ThreadInfo info=*(ThreadInfo*)param;
    int ss;
    struct sockaddr_in addr;
    ss = socket(PF_INET, SOCK_STREAM, 0);
    if (ss < 0) {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;         // IPv4
    addr.sin_port = htons(info.port); // ポート番号
    addr.sin_addr.s_addr = INADDR_ANY; // 任意のIPアドレスで待ち受け
    int bd = bind(ss, (struct sockaddr *)&addr, sizeof(addr));
    if (bd < 0) {
        perror("bind");
        close(ss);
        exit(1);
    }
    int lis = listen(ss, 10);  // 最大待ち行列数を10に設定
    if (lis < 0) {
        perror("listen");
        close(ss);
        exit(1);
    }
    struct sockaddr_in client_addr;
    socklen_t len_client = sizeof(client_addr);
    int client_s = accept(ss, (struct sockaddr *)&client_addr, &len_client);
    close(ss);
    if (client_s < 0) {
        perror("accept");
        close(ss);
        exit(1);
    }
    FILE* rec_fp = popen("rec -q -b 16 -c 1 -r 40000 -t raw -", "r");
    FILE* play_fp = popen("play -q -t raw -b 16 -c 1 -e s -r 40000 -", "w");
    FileData sentdata;
    FileData receiveddata;
    short recdata[SampleSize];
    complex double * X = calloc(sizeof(complex double), SampleSize);
    complex double * Y = calloc(sizeof(complex double), SampleSize);
    complex double * Z = calloc(sizeof(complex double), SampleSize);  // 受け取り用(0埋めしておく)
    while(1){
        zero_clear(Z,SampleSize);
        if(fread(&recdata, sizeof(short),SampleSize,rec_fp)<=0){
            break;
        }
        sample_to_complex(recdata,X, SampleSize);
        fft(X,Y,SampleSize);
        ssize_t send_data = send(client_s, Y, N* sizeof(complex double), 0);  // クライアントにデータを送信
        ssize_t read_data = read(client_s, Z, N* sizeof(complex double));  // クライアントからデータを受信
        ifft(Z,X,SampleSize);
        complex_to_sample(X,recdata,SampleSize);
        fwrite(recdata,sizeof(short),SampleSize, play_fp);
    }
}

/* param で指定したポート番号にコネクトしてサーバーと通信する(他のクライアントともサーバーを通じて通信する) */
void* client_thread(void* param){
    ThreadInfo info=*(ThreadInfo*)param;
    printf("%s:%d\n",info.ip,info.port);
    int s=socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    int aton = inet_aton(info.ip,&addr.sin_addr);
    if (aton == 0){
        perror("inet_aton");
    }
    addr.sin_port=htons(info.port);
    int ret=connect(s,(struct sockaddr*)&addr,sizeof(addr));
    FILE* rec_fp = popen("rec -q -b 16 -c 1 -r 40000 -t raw -", "r");
    FILE* play_fp = popen("play -q -t raw -b 16 -c 1 -e s -r 40000 -", "w");
    FileData sentdata;
    FileData receiveddata;
    short recdata[SampleSize];
    complex double * X = calloc(sizeof(complex double), SampleSize);
    complex double * Y = calloc(sizeof(complex double), SampleSize);
    complex double * Z = calloc(sizeof(complex double), SampleSize);  // 受け取り用(0埋めしておく)
    while(1){
        zero_clear(Z,SampleSize);
        if(fread(&recdata, sizeof(short),SampleSize,rec_fp)<=0){
            continue;
        }
        sample_to_complex(recdata,X, SampleSize);
        fft(X,Y,SampleSize);
        ssize_t send_data = send(s, Y, N* sizeof(complex double), 0);  // クライアントにデータを送信
        ssize_t read_data = read(s, Z, N* sizeof(complex double));  // クライアントからデータを受信
        ifft(Z,X,SampleSize);
        complex_to_sample(X,recdata,SampleSize);
        fwrite(recdata,sizeof(short),SampleSize, play_fp);
    }
}

/* サーバースレッドを立てる関数(サーバーを立てているわけではない; UIとの並列処理用) */
void GenServer(int port){
    pthread_t thread;
    ThreadInfo info;
    info.port=port;
    int ret = pthread_create(&thread, NULL,server_thread, &info);
    if (ret != 0) {
        printf("server thread failed\n");
        // pthread_createが失敗した場合は0以外の値を返す
        exit(1);
    }
    printf("server thread created\n");
}

/* クライアントスレッドを立てる関数(サーバーにコネクトしているわけではない; UIとの並列処理用) */
void GenClient(char* ip,int port){
    pthread_t thread;
    ThreadInfo info;
    strcpy(info.ip,ip);
    info.port=port;
    int ret = pthread_create(&thread, NULL,client_thread, &info);
    if (ret != 0) {
        printf("client thread failed\n");
        // pthread_createが失敗した場合は0以外の値を返す
        exit(1);
    }
    printf("client thread created\n");
}
