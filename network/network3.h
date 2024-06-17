#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include "dbdata.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "pthread.h"
#include "filedata.h"
#include "complex.h"
#include "../echo_cancel.h"
#include "../fft.h"
#ifdef NETWORK3_H
#else
#define NETWORK3_H
#define N (SampleSize*4000/48000)
typedef struct 
{
    char ip[256];
    int port;
    void* custom0;
    void* custom1;
}ThreadInfo;

void *toclient_audio_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    int socket=*(int *)info.custom0;
    complex double* global_spectrum=(complex double*)info.custom1;
    complex double recv_spectrum[N],old_spectrum[N],back_spectrum[N];
    zero_clear(recv_spectrum,N);
    zero_clear(old_spectrum,N);
    zero_clear(back_spectrum,N);
    while(1){
        int read_size=read(socket,recv_spectrum,sizeof(complex double)*N);
        if(read_size<=0)continue;
        for(int i=0;i<N;i++){
            global_spectrum[i]= global_spectrum[i] + recv_spectrum[i];
            global_spectrum[i]= global_spectrum[i] - old_spectrum[i];
            back_spectrum[i]= global_spectrum[i] - recv_spectrum[i];
        }
        write(socket,back_spectrum,sizeof(complex double)*N);
        for(int i=0;i<N;i++){
            old_spectrum[i]=recv_spectrum[i];
        }
    }
}
void *toclient_db_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    int socket=*(int *)info.custom0;
    DBData* db=(DBData*)info.custom1;
    while(1){

    }
}
void *server_audio_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    complex double* global_spectrum=(complex double*)info.custom0;
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
    while (1)
    {
        ThreadInfo client_info;
        struct sockaddr_in client_addr;
        socklen_t len_client = sizeof(client_addr);
        int client_s = accept(ss, (struct sockaddr *)&client_addr, &len_client);
        pthread_t thread;
        client_info.custom0=&client_s;
        client_info.custom1=global_spectrum;
        pthread_create(&thread,NULL,toclient_audio_thread,&client_info);
        printf("new user joined\n");

    }
}
void *server_db_thread(void *param)
{
    DBData* db=(DBData*)(sizeof(DBData));
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
    while (1)
    {
        ThreadInfo client_info;
        struct sockaddr_in client_addr;
        socklen_t len_client = sizeof(client_addr);
        int client_s = accept(ss, (struct sockaddr *)&client_addr, &len_client);
        pthread_t thread;
        client_info.custom0=&client_s;
        client_info.custom1=db;
        pthread_create(&thread,NULL,toclient_db_thread,&client_info);
        printf("new user joined\n");
    }
}
void *client_audio_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    FILE* rec_fp=(FILE*)info.custom0;
    FILE* play_fp=(FILE*)info.custom1;
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

    complex double * X = calloc(sizeof(complex double), SampleSize);
    complex double * Y = calloc(sizeof(complex double), SampleSize);

    while(1){
        short data[SampleSize];
        //zero_clear(Z, SampleSize);
        if(fread(data,sizeof(short),SampleSize,rec_fp)<=0){
            continue;
        }
        sample_to_complex(data,X, SampleSize);
        fft(X,Y,SampleSize);
        ssize_t send_data = write(s,Y,N* sizeof(complex double));
        ssize_t read_data = read(s,Y,N* sizeof(complex double));
        ifft(Y,X,SampleSize);
        complex_to_sample(X,data,SampleSize);
        fwrite(data,sizeof(short),SampleSize, play_fp);
    }
}
void *client_db_thread(void *param)
{
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
    while(1){
        char text[10];
        int n=read(s,text,10);
        if(n>0)
        printf("%s\n",text);
    }
}
void GenServer(int audio_port,int db_port){
    ThreadInfo info;
    complex double global_spectrum[SampleSize];
    info.custom0=global_spectrum;
    strcpy(info.ip,"0.0.0.0");
    info.port=audio_port;
    pthread_t thread;
    pthread_create(&thread,NULL,server_audio_thread,&info);

    ThreadInfo info1;
    strcpy(info1.ip,"0.0.0.0");
    info1.port=db_port;
    pthread_t thread1;
    pthread_create(&thread1,NULL,server_db_thread,&info1);
}
void GenClient(char* ip,int audio_port,int db_port){
    FILE* rec_fp = popen("rec -q -b 16 -c 1 -r 48000 -t raw -", "r");
    FILE* play_fp = popen("play -q -t raw -b 16 -c 1 -e s -r 48000 -", "w");
    ThreadInfo info;
    info.custom0=rec_fp;
    info.custom1=play_fp;
    strcpy(info.ip,ip);
    info.port=audio_port;
    pthread_t thread;
    pthread_create(&thread,NULL,client_audio_thread,&info);
    ThreadInfo info1;
    strcpy(info1.ip,ip);
    info1.port=db_port;
    pthread_t thread1;
    pthread_create(&thread1,NULL,client_db_thread,&info1);
}
#endif