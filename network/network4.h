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
#include "complex.h"
#include "../echo_cancel.h"
#include "../fft.h"
#include "dbrequest.h"
#ifdef NETWORK4_H
#else
#define NETWORK4_H
typedef struct 
{
    char ip[256];
    int port;
    void* config;
}ThreadInfo;

// DBサーバーの、クライアントを受け付けるスレッドの設定
typedef struct 
{
    DBData* db;
}Server_DBThread_Config;

// DBサーバーの、クライアントと通信を行うスレッドの設定
typedef struct 
{
    DBData* db;
    int socket_count;
    int sockets[MAX_USER_COUNT];
}Server_DBToClientThread_Config;

// DBサーバーに接続するクライアントのスレッドの設定
typedef struct 
{
    DBData* local_db;
    DBRequest* request;
}Client_DBThread_Config;


void *toclient_db_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    Server_DBToClientThread_Config* config=(Server_DBToClientThread_Config*)info.config;
    DBData* db=config->db;
    while(1){
        int updateflag=0;
        //read request
        for(int i=0;i<config->socket_count;i++){
            DBRequest request;
            int n=read(config->sockets[i],&request,sizeof(DBRequest));
            if(n<=0){

            }
            else{
                updateflag=1;
                if(strcmp(request.method,"ADD")==0){
                    RoomData* room=db->rooms+request.room_id;
                    Room_Add_Message(room,Create_MessageData(request.user_id,request.message));
                }
                else if(strcmp(request.method,"JOIN")==0){

                }
            }
        }
        for(int i=0;i<config->socket_count;i++){
            write(config->sockets[i],db,sizeof(DBData));
        }
        if(updateflag){
            DB_Save(db,"volume/db");
        }
        usleep(50000);
    }
    
}
void *server_db_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    Server_DBThread_Config * config = (Server_DBThread_Config*)info.config;
    struct sockaddr_in client_addr;
    socklen_t len_client = sizeof(client_addr);
    printf("dbserver:0.0.0.0:%d\n",info.port);
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

    ThreadInfo db_toclient_thread_info;
    Server_DBToClientThread_Config db_toclient_thread_config;
    db_toclient_thread_config.db=config->db;
    db_toclient_thread_info.config=&db_toclient_thread_config;
    pthread_t thread;
    pthread_create(&thread,NULL,toclient_db_thread,&db_toclient_thread_info);
     
    while (1)
    {
        int s= accept(ss, (struct sockaddr *)&client_addr, &len_client);
        //ソケットをノンブロッキングに設定
        fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK);
        db_toclient_thread_config.sockets[db_toclient_thread_config.socket_count]=s;
        db_toclient_thread_config.socket_count++;
        printf("new user joined%d\n", db_toclient_thread_config.socket_count);
    }
    close(ss);
}
void *client_db_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    Client_DBThread_Config* config=(Client_DBThread_Config*)info.config;
    printf("%s:%d\n",info.ip,info.port);
    int s=socket(PF_INET,SOCK_STREAM,0);
    if(s<0){
         perror("socket");
         exit(1);
    }
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    int aton = inet_aton(info.ip,&addr.sin_addr);
    if (aton == 0){
        perror("inet_aton");
        exit(1);
    }
    addr.sin_port=htons(info.port);
    int ret=connect(s,(struct sockaddr*)&addr,sizeof(addr));
    while(1){
        DBData db_from_server;
        int rn=read(s,&db_from_server,sizeof(DBData));
        if(rn>0){
            *config->local_db=db_from_server;
        }
        if(config->request->method[0]==0){

        }
        else{
            int wn=write(s,config->request,sizeof(DBRequest));
            memset(config->request,0,sizeof(DBRequest));
        }
    }
}
void GenServer(int audio_port,int db_port){
    //generate server db
    DBData* db=(DBData*)malloc(sizeof(DBData));
    //ディスクからdbを読み込む
    DB_Load(db,"volume/db");
    //testdata
    // complex double spectrums[MAX_USER_COUNT][SampleSize];
    // info.custom0=spectrums;
    /*
    ThreadInfo* info=malloc(sizeof(ThreadInfo));
    strcpy(info->ip,"0.0.0.0");
    info->port=audio_port;
    info->custom0=db;
    pthread_t* thread=malloc(sizeof(pthread_t));
    pthread_create(thread,NULL,server_audio_thread,info);
    */
    {
        ThreadInfo* info=(ThreadInfo*)malloc(sizeof(ThreadInfo));
        Server_DBThread_Config* config=(Server_DBThread_Config*)malloc(sizeof(Server_DBThread_Config));
        config->db=db;
        strcpy(info->ip,"0.0.0.0");
        info->port=db_port;
        info->config=config;
        pthread_t* thread=(pthread_t*)malloc(sizeof(pthread_t));
        pthread_create(thread,NULL,server_db_thread,info);
    }
}
//dbはclientが今持っているdb.serverからの受信を受けて更新される. requestはdbに対する操作、受理されると0になる
void GenClient(char* ip,int audio_port,int db_port, int * status,DBData* db,DBRequest* request){
    FILE* rec_fp = popen("rec -q -b 16 -c 1 -r 48000 -t raw - 2>log_rec.txt", "r");
    FILE* play_fp = popen("play -q -t raw -b 16 -c 1 -e s -r 48000 - 2>log_play.txt", "w");
/*
    ThreadInfo* info=malloc(sizeof(ThreadInfo));
    strcpy(info->ip,ip);
    info->custom0=rec_fp;
    info->custom1=play_fp;
    info->status = status;
    info->port=audio_port;
    pthread_t* thread=malloc(sizeof(pthread_t));
    pthread_create(thread,NULL,client_audio_thread,info);
*/
    {
        ThreadInfo* info=(ThreadInfo*)malloc(sizeof(ThreadInfo));
        Client_DBThread_Config* config=(Client_DBThread_Config*)malloc(sizeof(Client_DBThread_Config));
        config->local_db=db;
        config->request=request;
        strcpy(info->ip,ip);

        info->port=db_port;
        info->config=config;
        pthread_t* thread=(pthread_t*)malloc(sizeof(pthread_t));
        pthread_create(thread,NULL,client_db_thread,info);
    }
}
#endif