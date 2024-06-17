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
#include "../echo_cancel.h"
#include "../fft.h"
#ifdef NETWORK2_H
#else
#define NETWORK2_H
#define N SampleSize*4000/48000

pthread_mutex_t mutex;

typedef struct 
{
    char ip[256];
    int port;

}ThreadInfo;

// グローバルなスペクトルデータ
complex double global_spectrum[N];
// zero_clear(global_spectrum,N);  // global_spectrumを0で初期化

// サーバーのクライアントとの通信スレッド関数
//  引数で受け取ったソケットを用いた通信のスレッドを立てる
//      recv_spectrum, old_spectrum, back_spectrumを初期化する
//      eadでクライアントから受け取ったデータをrecv_spectrumに格納する
//      global_spectrumにrecv_spectrumを足し、old_spectrumを引く
//      global_spectrumからrecv_spectrumを引いたものをback_spectrumに格納する
//      back_spectrumをsendする
//      recv_spectrumをold_spectrumにコピーする
//  以上をreadで受け取ったデータが0になるまで繰り返す
// スレッドを終了する
// 実装例
void* toclient_thread(void* param){
    int client_s=*(int*)param;
    complex double recv_spectrum[N],old_spectrum[N],back_spectrum[N];
    zero_clear(recv_spectrum,N);
    zero_clear(old_spectrum,N);
    zero_clear(back_spectrum,N);
    while(1){
        int read_size=read(client_s,recv_spectrum,sizeof(complex double)*N);
        if(read_size<=0)continue;;
        // global_spectrumをロックする
        //pthread_mutex_lock(&mutex);
        print_complex(stdout,global_spectrum,N);

        for(int i=0;i<N;i++){
            global_spectrum[i]= global_spectrum[i] + recv_spectrum[i];
            global_spectrum[i]= global_spectrum[i] - old_spectrum[i];
            back_spectrum[i]= global_spectrum[i] - recv_spectrum[i];
        }
        // global_spectrumをアンロックする
        //pthread_mutex_unlock(&mutex);
        // send
        write(client_s,back_spectrum,sizeof(complex double)*N);
        for(int i=0;i<N;i++){
            old_spectrum[i]=recv_spectrum[i];
        }
    }
    close(client_s);
    pthread_exit(NULL);
}


// サーバースレッド関数
//  スレッドを立てる
//   バインド・リッスン用のソケットを作成する
//   global_spectrumを初期化する
//   アクセプトされたソケットを取得して、そのクライアントとの通信スレッドを立てて通信する関数を呼び出し、joinする
//   スレッドを終了する
// 実装例
void* server_thread(void* param){
    //pthread_mutex_init(&mutex,NULL);
    ThreadInfo info=*(ThreadInfo*)param;
    int ss;                                 // バインド・リッスン用のソケット
    struct sockaddr_in addr;
    ss = socket(PF_INET, SOCK_STREAM, 0);
    if (ss < 0) {
        perror("socket");
        exit(1);
    }
    addr.sin_family = AF_INET;         // IPv4
    addr.sin_port = htons(info.port); // ポート番号
    // addr.sin_port = htons(0)
    addr.sin_addr.s_addr = INADDR_ANY; // 任意のIPアドレスを待ち受け
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
    while(1){
        struct sockaddr_in client_addr;
        socklen_t len_client = sizeof(client_addr);
        int client_s = accept(ss, (struct sockaddr *)&client_addr, &len_client);
        pthread_t thread;
        pthread_create(&thread,NULL,toclient_thread,&client_s);
    }
    close(ss);
    pthread_exit(NULL);
}


// クライアントスレッド関数
//   引数として受けとったIPアドレスをもとにソケットを作成して、connectでサーバーに接続する(ポート番号は、htons(0)でランダムに割り当て)
//   複素数配列 X, Y, Z, L 、short型の配列 data を初期化する
//   録音と再生のためのデバイスをオープンするパイプを作成する(popen)
//     freadしてdataにデータを読み込む
//     dataを複素数配列に変換してXに格納する
//     XをLを用いてエコーキャンセルして更新する
//     XをFFTしてYに格納する
//     Yをサーバーにsendする
//     サーバーからreadしてZに格納する
//     ZをIFFTしてLに格納する
//     Lをdataに変換してfwriteする
//   以上をdataが読み込めなくなるまで繰り返す
//   スレッドを終了する
// 実装例
void* client_thread(void* param){
    // サーバーの情報
    ThreadInfo info=*(ThreadInfo*)param;
    printf("%s:%d\n",info.ip,info.port);
    // ソケット作成
    int s=socket(PF_INET,SOCK_STREAM,0);
    struct sockaddr_in addr;
    addr.sin_family=AF_INET;
    // int aton = inet_aton(info.ip,&addr.sin_addr);
    int aton = inet_aton(info.ip,&addr.sin_addr);
    if (aton == 0){
        perror("inet_aton");
    }
    addr.sin_port=htons(info.port);
    // コネクト
    int ret=connect(s,(struct sockaddr*)&addr,sizeof(addr));
    // 入出力ファイルポインタ
    FILE* rec_fp = popen("rec -q -b 16 -c 1 -r 48000 -t raw -", "r");
    FILE* play_fp = popen("play -q -t raw -b 16 -c 1 -e s -r 48000 -", "w");
    // データ
    complex double * X = calloc(sizeof(complex double), SampleSize);
    complex double * X_canceled = calloc(sizeof(complex double), SampleSize);
    complex double * Y = calloc(sizeof(complex double), SampleSize);
    complex double * Z = calloc(sizeof(complex double), SampleSize);  // 受け取り用(0埋めしておく)
    complex double * L = calloc(sizeof(complex double), SampleSize);  // エコーキャンセル時に用いる
    // エコーキャンセル用
    complex double filter[FILTER_LENGTH];
    complex double estimated_echo[SampleSize];
    init_filter(filter, FILTER_LENGTH);

    while(1){
        short data[SampleSize];
        zero_clear(Z, SampleSize);
        if(fread(data,sizeof(short),SampleSize,rec_fp)<=0){
            continue;
        }
        sample_to_complex(data,X, SampleSize);

        // /* エコーキャンセル */
        // // echo_cancel(X,L,SampleSize);
        // estimate_echo(filter,X,estimated_echo,SampleSize);
        // for(int i=0;i<SampleSize;i++){
        //     X_canceled[i] =  X[i] - estimated_echo[i];
        // }
        // update_filter(filter,X,X_canceled,SampleSize);

        /* エコーキャンセルしない場合 */
        for (int i=0; i<SampleSize;i++){
            X_canceled[i] = X[i];
        }

        fft(X_canceled,Y,SampleSize);
        ssize_t send_data = write(s,Y,N* sizeof(complex double));
        ssize_t read_data = read(s,Z,N* sizeof(complex double));
        ifft(Z,L,SampleSize);
        complex_to_sample(L,data,SampleSize);
        print_complex(stdout,Z,N);
        fwrite(data,sizeof(short),SampleSize, play_fp);
    }
    close(s);
    pthread_exit(NULL);
}


// サーバーのコマンダー関数
//  スレッドを立ててサーバースレッド関数を呼び出しjoinするとともに、クライアントスレッド関数を呼び出してスレッドを立ててjoinする
// 実装例
void GenServer(int port){
    zero_clear(global_spectrum,N);
    ThreadInfo info;
    strcpy(info.ip,"0.0.0.0");
    info.port=port;
    pthread_t thread;
    pthread_create(&thread,NULL,server_thread,&info);
    // pthread_join(thread,NULL);
    pthread_t thread2;
    pthread_create(&thread2,NULL,client_thread,&info);
    // pthread_join(thread2,NULL);
}


// クライアントのコマンダー関数
// 引数で受け取ったIPアドレスでクライアントスレッド関数を呼び出す
// 実装例
void GenClient(const char* ip, int port){
    pthread_t thread;
    // 接続先のサーバーの情報を構造体に格納
    ThreadInfo info;
    strcpy(info.ip,ip);
    info.port=port;
    // クライアントスレッドを立てる
    pthread_create(&thread,NULL,client_thread,&info);
}
#endif