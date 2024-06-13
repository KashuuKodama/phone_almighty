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

#include "fft.h"
#include "echo_cancel.h"

#define SIZE 4096
int server(int port){
 int ss;
    struct sockaddr_in addr;
    ss = socket(PF_INET, SOCK_STREAM, 0);
    if (ss < 0) {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;         // IPv4
    addr.sin_port = htons(port); // ポート番号
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
    return client_s;
}
int client(const char* ip,int port){
    printf("%s:%d\n",ip,port);
    int s=socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    int aton = inet_aton(ip,&addr.sin_addr);
    if (aton == 0){
        perror("inet_aton");
        return 1;
    }
    addr.sin_port=htons(port);
    int ret=connect(s,(struct sockaddr*)&addr,sizeof(addr));
    return s;
}
int main(int argc, char *argv[]) {
    // サンプリング周波数
    int fs = 48000;
    // 音声の周波数帯の離散スペクトル値の数
    int n = SIZE*4000/fs;
    if (n>SIZE){
        n=SIZE;
    }
    int s=0;
    if (argc == 3) {
        s=client(argv[1],atoi(argv[2]));
    }
    if (argc == 2) {
        s=server(atoi(argv[1]));
    }
    FILE *rec_fp = popen("rec -q -b 16 -c 1 -r 48000 -t raw -", "r");
    FILE *play_fp = popen("play -t raw -b 16 -c 1 -e s -r 48000 -", "w");
    if (rec_fp == NULL) {
        perror("popen");
        close(s);
        return 1;
    }

    // 標準入力から取り込んだデータ
    complex double * X = calloc(sizeof(complex double), SIZE);
    // 標準出力から出力したデータ
    complex double * L = calloc(sizeof(complex double), SIZE);  // エコーキャンセル時に用いる
    // スペクトル通信用
    complex double * Y = calloc(sizeof(complex double), SIZE);
    complex double * Z = calloc(sizeof(complex double), SIZE);  // 受け取り用(0埋めしておく)
    // エコーキャンセル用
    complex double filter[FILTER_LENGTH];
    complex double estimated_echo[SIZE];
    init_filter(filter, FILTER_LENGTH);
    
    // recから読み込んだデータをクライアントに送信
    while (1) {
        short data[SIZE];
        zero_clear(Z,SIZE);
        if(fread(&data, sizeof(short),SIZE,rec_fp)<=0){
            break;
        }
        sample_to_complex(data,X, SIZE);

        /* XをLを用いてエコーキャンセル*/
        estimate_echo(filter, L, estimated_echo, SIZE);
        for (int i = 0; i < SIZE; i++) {
            X[i] -= estimated_echo[i];
        }
        update_filter(filter, Z, X, SIZE);

        fft(X,Y,SIZE);

        // ssize_t send_data = send(s,&data, SIZE* sizeof(short), 0);  // クライアントにデータを送信
        // ssize_t read_data = read(s,&data, SIZE* sizeof(short));  // クライアントからデータを受信

        ssize_t send_data = send(s, Y, n* sizeof(complex double), 0);  // クライアントにデータを送信
        ssize_t read_data = read(s, Z, n* sizeof(complex double));  // クライアントからデータを受信
        
        // print_complex(stdout,Z,SIZE);
        ifft(Z,L,SIZE);

        // fwrite(&data,sizeof(short),SIZE, play_fp);
        complex_to_sample(L,data,SIZE);
        fwrite(&data,sizeof(short),SIZE, play_fp);

        //write(1,&data, sizeof(data));  // recからデータを読み込む
    }
    pclose(rec_fp);
    close(s);
    return 0;
}
