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
#include "dbrequest.h"
#ifdef NETWORK3_H
#else
#define NETWORK3_H
#define N (SampleSize*8000/48000)
typedef struct 
{
    char ip[256];
    int port;
    void* custom0;
    void* custom1;
    void* custom2;
    int* status;
}ThreadInfo;

void *toclient_audio_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    int* sockets=(int *)info.custom0;
    int* user_count=(int *)info.custom1;
    DBData* db=(DBData*)info.custom2;
    complex double global_spectrum[N];
    complex double spectrums[MAX_USER_COUNT][N];
    while(1){
        zero_clear(global_spectrum,N);
        /* データの受け取り(接続していないクライアントも含めて) */
        for(int i=0;i<*user_count;i++){
            int read_size=read(sockets[i],spectrums[i],sizeof(complex double)*N);
            // printf("r\n");
            /* データを受け取っていない場合、サーバ側ではミュートの扱いにする */
            if(read_size<=0){
                db->statuses[i]=1;
                printf("user %d is muted\n",i);
                continue;
            }
            db->statuses[i]=0;      // データを受け取ったらステータスを0(アンミュート)にする
            for(int j=0;j<N;j++){
                global_spectrum[j]+=spectrums[i][j];
            }
            
        }
        /* データの送信 */
        for(int i=0;i<*user_count;i++){
            for(int j=0;j<N;j++){
                spectrums[i][j]=global_spectrum[j]-spectrums[i][j];
            }
            // printf("w\n");
            if (db->statuses[i]==1){
                continue;
            }
            write(sockets[i],spectrums[i],sizeof(complex double)*N);
            // printf("w\n");
        }
    }
}
void *toclient_db_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    int* sockets=(int *)info.custom0;
    int* user_count=(int *)info.custom1;
    DBData* db=(DBData*)info.custom2;
    while(1){
        //read request
        for(int i=0;i<*user_count;i++){
            DBRequest request;
            int n=read(sockets[i],&request,sizeof(DBRequest));
            if(n<=0){

            }
            else{
                if(strcmp(request.method,"ADD")==0){
                    RoomData* room=db->rooms+request.room;
                    if(room->length==MAX_MESSAGE_COUNT){
                        //delete old data
                        memmove(room->messages,room->messages+1,room->length-1);
                        strcpy(room->messages[room->length-1].text,request.message);
                        strcpy(room->messages[room->length-1].user,request.user);
                    }
                    else{
                        room->length++;
                        strcpy(room->messages[room->length-1].text,request.message);
                        strcpy(room->messages[room->length-1].user,request.user);
                    }
                }
                else if(strcmp(request.method,"JOIN")==0){

                }
            }
        }
        for(int i=0;i<*user_count;i++){
            write(sockets[i],db,sizeof(DBData));
        }
    }
}
void *server_audio_thread(void *param)
{
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
    int sockets[MAX_USER_COUNT];
    int user_count=0;
    ThreadInfo client_audio_thread_info;
    client_audio_thread_info.custom0=sockets;
    client_audio_thread_info.custom1=&user_count;
    client_audio_thread_info.custom2=info.custom0;
    pthread_t thread;
    pthread_create(&thread,NULL,toclient_audio_thread,&client_audio_thread_info);
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t len_client = sizeof(client_addr);
        sockets[user_count]= accept(ss, (struct sockaddr *)&client_addr, &len_client);
        user_count++;
        printf("new user joined\n");

    }
}
void *server_db_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    DBData * db = (DBData*)info.custom0;
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
    int sockets[MAX_USER_COUNT];
    int user_count=0;
    ThreadInfo client_db_thread_info;
    client_db_thread_info.custom0=sockets;
    client_db_thread_info.custom1=&user_count;
    client_db_thread_info.custom2=info.custom0;
    pthread_t thread;
    pthread_create(&thread,NULL,toclient_db_thread,&client_db_thread_info);
    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t len_client = sizeof(client_addr);
        sockets[user_count]= accept(ss, (struct sockaddr *)&client_addr, &len_client);
        user_count++;
        printf("new user joined\n");
    }
}
void *client_audio_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    FILE* rec_fp=(FILE*)info.custom0;
    FILE* play_fp=(FILE*)info.custom1;
    printf("audio%s:%d\n",info.ip,info.port);
    int s=socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    int aton = inet_aton(info.ip,&addr.sin_addr);
    if (aton == 0){
        perror("inet_aton");
    }
    addr.sin_port=htons(info.port);
    int ret=connect(s,(struct sockaddr*)&addr,sizeof(addr));

    /* ミュート, アンミュートのステート(ブール型) */
    int * mute = info.status;   // 0: unmute, 1: mute

    complex double * X = calloc(sizeof(complex double), SampleSize);
    complex double * X_canceled = calloc(sizeof(complex double), SampleSize);
    complex double * Y = calloc(sizeof(complex double), SampleSize);
    complex double * L = calloc(sizeof(complex double), SampleSize);  // エコーキャンセル時に用いる
    /* エコーキャンセル用 */
    complex double filter[FILTER_LENGTH];
    complex double estimated_echo[SampleSize];
    init_filter(filter, FILTER_LENGTH);
    /* データ送受信フラグ　*/
    ssize_t send_data, read_data;

    while(1){
        short data[SampleSize];
        memset(data, 0, sizeof(short)*SampleSize);
        /* データが届いていないもしくはミュートの場合は音を収録しない */
        if(fread(data,sizeof(short),SampleSize,rec_fp)<=0){
            continue;
        } else if (*mute == 0) {
            sample_to_complex(data,X, SampleSize);

            /* エコーキャンセル */
            estimate_echo(filter,L,estimated_echo,SampleSize);
            // print_complex(stderr,estimated_echo,SampleSize);
            for(int i=0;i<SampleSize;i++){
                X_canceled[i] =  X[i] - estimated_echo[i];
            }
            update_filter(filter,L,X_canceled,SampleSize);
            // print_complex(stderr,filter,FILTER_LENGTH);

            /* エコーキャンセルしない場合 */
            // for (int i=0; i<SampleSize;i++){
            //     X_canceled[i] = X[i];
            // }
            // print_complex(stderr,X_canceled,SampleSize);
            // fft(X,Y,SampleSize);
            fft(X_canceled,Y,SampleSize);
            // printf("send\n");
            // print_complex(stderr,Y,SampleSize);
            send_data = write(s,Y,N* sizeof(complex double));
        }
        zero_clear(Y,SampleSize);
        read_data = read(s,Y,N* sizeof(complex double));
        // printf("read\n");
        // print_complex(stderr,Y,SampleSize);
        // ifft(Y,X,SampleSize);
        // complex_to_sample(X,data,SampleSize);
        ifft(Y,L,SampleSize);
        // print_complex(stderr,L,SampleSize);
        complex_to_sample(L,data,SampleSize);
        fwrite(data,sizeof(short),SampleSize, play_fp);
    }
}
void *client_db_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    DBData* db=(DBData*)info.custom0;
    DBRequest* request=(DBRequest*)info.custom1;
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
        int rn=read(s,db,sizeof(DBData));
        if(request->method[0]==0){

        }
        else{
            int wn=write(s,request,sizeof(DBRequest));
            memset(request,0,sizeof(DBRequest));
        }
    }
}
void GenServer(int audio_port,int db_port){
    ThreadInfo info;
    DBData* db=(DBData*)malloc(sizeof(DBData));
    //testdata
    db->rooms->length=1;
    strcpy(db->rooms->messages[0].text,"hello");
    strcpy(db->rooms->messages[0].user,"hello");

    // complex double spectrums[MAX_USER_COUNT][SampleSize];
    // info.custom0=spectrums;
    info.custom0=db;
    strcpy(info.ip,"0.0.0.0");
    info.port=audio_port;
    pthread_t thread;
    pthread_create(&thread,NULL,server_audio_thread,&info);

    ThreadInfo info1;
    strcpy(info1.ip,"0.0.0.0");
    info1.port=db_port;
    info1.custom0=db;
    pthread_t thread1;
    pthread_create(&thread1,NULL,server_db_thread,&info1);
    //wait till thread is created
    sleep(1);
}
//dbはclientが今持っているdb.serverからの受信を受けて更新される. requestはdbに対する操作、受理されると0になる
void GenClient(char* ip,int audio_port,int db_port, int * status,DBData* db,DBRequest* request){
    FILE* rec_fp = popen("rec -q -b 16 -c 1 -r 48000 -t raw -", "r");
    FILE* play_fp = popen("play -q -t raw -b 16 -c 1 -e s -r 48000 -", "w");
    ThreadInfo info;
    info.custom0=rec_fp;
    info.custom1=play_fp;
    info.status = status;
    strcpy(info.ip,ip);
    info.port=audio_port;
    pthread_t thread;
    pthread_create(&thread,NULL,client_audio_thread,&info);
    ThreadInfo info1;
    strcpy(info1.ip,ip);
    info1.port=db_port;
    info1.status = status;
    info1.custom0=db;
    info1.custom1=request;
    pthread_t thread1;
    pthread_create(&thread1,NULL,client_db_thread,&info1);
    //wait till thread is created
    sleep(1);
}
#endif