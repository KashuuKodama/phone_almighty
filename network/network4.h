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
#define SampleSize 8192
#define N (SampleSize*8000/48000)
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
    DBRequests* requests;
}Client_DBThread_Config;

// Audioサーバーの、クライアントを受け付けるスレッドの設定
typedef struct 
{
    DBData* db;
}Server_AudioThread_Config;

// Audioサーバーの、クライアントと通信を行うスレッドの設定
typedef struct 
{
    int socket_count;
    int sockets[MAX_USER_COUNT];
}Server_AudioToClientThread_Config;

// Audioサーバーに接続するクライアントのスレッドの設定
typedef struct 
{
    DBData* local_db;
    FILE* rec_fp;
    FILE* play_fp;
}Client_AudioThread_Config;

void *toclient_audio_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    Server_AudioToClientThread_Config* config=(Server_AudioToClientThread_Config*)info.config;
    complex double global_spectrum[N];
    complex double spectrums[MAX_USER_COUNT][N];
    while(1){
        zero_clear(global_spectrum,N);
        /* データの受け取り(接続していないクライアントも含めて) */
        for(int i=0;i<config->socket_count;i++){
            int read_size=read(config->sockets[i],spectrums[i],sizeof(complex double)*N);
            // printf("r\n");
            /* データを受け取っていない場合、サーバ側ではミュートの扱いにする */
            //↑のやり方ではミュート解除してからスペクトルの位置が合わなくなるのでクライアントから0埋めしたものを送るようにした
            // if(read_size<=0){
            //     // db->statuses[i]=1;
            //     printf("user %d is muted\n",i);
            //     continue;
            // }
            // db->statuses[i]=0;      // データを受け取ったらステータスを0(アンミュート)にする
            for(int j=0;j<N;j++){
                global_spectrum[j]+=spectrums[i][j];
            }
            
        }
        /* データの送信 */
        for(int i=0;i<config->socket_count;i++){
            for(int j=0;j<N;j++){
                spectrums[i][j]=global_spectrum[j]-spectrums[i][j];
            }
            // printf("w\n");
            // if (db->statuses[i]==1){
                // continue;
            // }
            write(config->sockets[i],spectrums[i],sizeof(complex double)*N);
            // printf("w\n");
        }
    }
    
}

void *server_audio_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    Server_AudioThread_Config * config = (Server_AudioThread_Config*)info.config;
    struct sockaddr_in client_addr;
    socklen_t len_client = sizeof(client_addr);
    printf("audioserver:0.0.0.0:%d\n",info.port);
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

    ThreadInfo audio_toclient_thread_info;
    Server_AudioToClientThread_Config audio_toclient_thread_config;
    audio_toclient_thread_info.config=&audio_toclient_thread_config;
    pthread_t thread;
    pthread_create(&thread,NULL,toclient_audio_thread,&audio_toclient_thread_info);
     
    while (1)
    {
        int s= accept(ss, (struct sockaddr *)&client_addr, &len_client);
        audio_toclient_thread_config.sockets[audio_toclient_thread_config.socket_count]=s;
        audio_toclient_thread_config.socket_count++;
        printf("new user joined%d\n", audio_toclient_thread_config.socket_count);
    }
    close(ss);
}
void *client_audio_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    Client_AudioThread_Config* config=(Client_AudioThread_Config*)info.config;
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

    complex double * X = calloc(sizeof(complex double), SampleSize);
    complex double * X_canceled = calloc(sizeof(complex double), SampleSize);
    complex double * Y = calloc(sizeof(complex double), SampleSize);
    complex double * L = calloc(sizeof(complex double), SampleSize);  // エコーキャンセル時に用いる
    /* エコーキャンセル用 */
    complex double filter[FILTER_LENGTH];
    complex double estimated_echo[SampleSize];
    init_filter(filter, FILTER_LENGTH);
    while(1){
        ssize_t send_data, read_data;
        short data[SampleSize];
        memset(data, 0, sizeof(short)*SampleSize);
        int n=fread(data,sizeof(short),SampleSize,config->rec_fp);
        /* 参加していないデータが届いていないもしくはミュートの場合は音を収録しない */
        //代わりに0を送る
        AudioStatus status=config->local_db->statuses[config->local_db->user_id];
        if(n<=0||
        status.muted
        ||!status.joined
        ||config->local_db->current_room_id!=config->local_db->phone_room_id
        ||config->local_db->current_room_id<0){
            complex double zero[N];
            write(s,zero,N* sizeof(complex double));
        } 
        else {
            sample_to_complex(data,X, SampleSize);
            /* エコーキャンセル */
            // estimate_echo(filter,L,estimated_echo,SampleSize);
            // print_complex(stderr,estimated_echo,SampleSize);
            // for(int i=0;i<SampleSize;i++){
            //     X_canceled[i] =  X[i] - estimated_echo[i];
            // }
            // update_filter(filter,L,X_canceled,SampleSize);
            // print_complex(stderr,filter,FILTER_LENGTH);

            /* エコーキャンセルしない場合 */
            for (int i=0; i<SampleSize;i++){
                X_canceled[i] = X[i];
            }
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
        if(status.joined)
        fwrite(data,sizeof(short),SampleSize, config->play_fp);
    }
}

void *toclient_db_thread(void *param)
{
    ThreadInfo info=*(ThreadInfo*)param;
    Server_DBToClientThread_Config* config=(Server_DBToClientThread_Config*)info.config;
    DBData* db=config->db;
    while(1){
        int updateflag=0;
        //read requests
        for(int i=0;i<config->socket_count;i++){
            DBRequests requests;
            int n=read(config->sockets[i],&requests,sizeof(DBRequests));
            if(n==sizeof(DBRequests)){
                for(int i=0;i<requests.requests_length;i++){
                    DBRequest request=requests.requests[i];
                    if(strcmp(request.method,"CHAT")==0){
                        RoomData* room=db->rooms+request.room_id;
                        Room_Add_Message(room,Create_MessageData(request.user_id,request.message));
                        updateflag=1;
                    }
                    else if(strcmp(request.method,"ADD_USER")==0){
                        UserData* user=(UserData*)request.message;
                        DB_Add_User(db,*user);
                        updateflag=1;
                    }
                    else if(strcmp(request.method,"EDIT_USER")==0){
                        UserData* user=(UserData*)request.message;
                        DB_Edit_User(db,request.user_id,*user);
                        updateflag=1;
                    }
                    else if(strcmp(request.method,"JOIN")==0){
                        updateflag=1;
                    }
                    else if(strcmp(request.method,"PHONE")==0){
                        db->phone_room_id=request.room_id;
                        updateflag=1;
                    }
                    else if(strcmp(request.method,"STATUS")==0){
                        db->statuses[request.user_id]=*(AudioStatus*)request.message;
                        updateflag=1;
                    }
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

        int current_user=config->local_db->user_id;
        int current_room=config->local_db->current_room_id;
        AudioStatus current_status=config->local_db->statuses[config->local_db->user_id];

        if(rn>0){
            *config->local_db=db_from_server;
            //serverからuseridの変更を禁ずる
            config->local_db->user_id=current_user;
            //serverから選択中のトークの変更を禁ずる
            config->local_db->current_room_id=current_room;
            //serverからaudio statusの変更を禁ずる
            config->local_db->statuses[config->local_db->user_id]=current_status;
        }
        if(config->requests->requests_length==0){

        }
        else{
            int wn=write(s,config->requests,sizeof(DBRequests));
            memset(config->requests,0,sizeof(DBRequests));
        }
    }
}
void GenServer(int audio_port,int db_port){
    //generate server db
    DBData* db=(DBData*)malloc(sizeof(DBData));
    //ディスクからdbを読み込む
    DB_Load(db,"volume/db");
    //初めはどのトークも会話してない
    db->phone_room_id=-1;
    //初めはトークを選択してない
    db->current_room_id=-1;
    //初めはみな通話に参加してない
    memset(db->statuses,0,sizeof(AudioStatus)*MAX_USER_COUNT);
    {
        ThreadInfo* info=(ThreadInfo*)malloc(sizeof(ThreadInfo));
        Server_AudioThread_Config* config=(Server_AudioThread_Config*)malloc(sizeof(Server_AudioThread_Config));
        config->db=db;
        strcpy(info->ip,"0.0.0.0");
        info->port=audio_port;
        info->config=config;
        pthread_t* thread=(pthread_t*)malloc(sizeof(pthread_t));
        pthread_create(thread,NULL,server_audio_thread,info);
    }

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
void GenClient(char* ip,int audio_port,int db_port,DBData* db,DBRequests* requests){

    //初めはどのトークも会話してない
    db->phone_room_id=-1;
    //初めはトークを選択してない
    db->current_room_id=-1;

    FILE* rec_fp = popen("rec -q -b 16 -c 1 -r 48000 -t raw - 2>log_rec.txt", "r");
    FILE* play_fp = popen("play -q -t raw -b 16 -c 1 -e s -r 48000 - 2>log_play.txt", "w");
    {
        ThreadInfo* info=(ThreadInfo*)malloc(sizeof(ThreadInfo));
        Client_AudioThread_Config* config=(Client_AudioThread_Config*)malloc(sizeof(Client_AudioThread_Config));
        config->local_db=db;
        config->play_fp=play_fp;
        config->rec_fp=rec_fp;
        strcpy(info->ip,ip);
        info->port=audio_port;
        info->config=config;
        pthread_t* thread=(pthread_t*)malloc(sizeof(pthread_t));
        pthread_create(thread,NULL,client_audio_thread,info);
    }
    //dbthread
    {
        ThreadInfo* info=(ThreadInfo*)malloc(sizeof(ThreadInfo));
        Client_DBThread_Config* config=(Client_DBThread_Config*)malloc(sizeof(Client_DBThread_Config));
        config->local_db=db;
        config->requests=requests;
        strcpy(info->ip,ip);

        info->port=db_port;
        info->config=config;
        pthread_t* thread=(pthread_t*)malloc(sizeof(pthread_t));
        pthread_create(thread,NULL,client_db_thread,info);
    }
}
#endif