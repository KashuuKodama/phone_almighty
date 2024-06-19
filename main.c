#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "camera.h"
#include "input.h"
#include "primitives.h"
#include "textures.h"
#include "timer.h"
#include "startmenu.h"
#include "chatscene.h"
#include "roomsscene.h"
#include "network/network4.h"
#include "network/dbdata.h"
#include "network/dbrequest.h"
#define PI 3.14f
#define Deg2Rad PI/180

int main(int argc, char *argv[]){
    signal(SIGPIPE, SIG_IGN);
    DBData db;
    DBRequest request;
    //とりあえずゲストユーザー
    db.user_id=0;
    //初めの入力は消去
    int tmp[100];
    getkeys(tmp,100);

    if (argc == 4) {
        GenClient(argv[1],atoi(argv[2]),atoi(argv[3]),&db,&request);
    }
    if (argc == 3) {
        GenServer(atoi(argv[1]),atoi(argv[2]));
        sleep(3);
        GenClient("0.0.0.0",atoi(argv[1]),atoi(argv[2]),&db,&request);
    }
    Camera* camera=SetupCamera(Deg2Rad*3,280,210);
    camera->pos.z=-1;
    // while(1){
    //     // int a[3];
    //     // int n=getkeys(a,3);
    //     // for(int i=0;i<n;i++){
    //     //     printf("%d\n",a[i]);
    //     // }
    // }
    //起動画面　
    goto start;
start:
    StartMenu(camera,&db,&request);
    goto rooms;
rooms:
    {
        int ret=RoomsScene(camera,&db,&request);
        if(ret==1){
            goto chat;
        }
        else{
            goto start;
        }
    }
chat:
    {
        int ret=ChatScene(camera,&db,&request,0);
        if(ret==1){
            goto call;
        }
        else{
            goto rooms;
        }
    }
call:
    {
        int ret=CallScene(camera,&db,&request,0);
        if(ret==1){
            goto chat;
        }
        FreeCamera(camera);
        free(camera);
    }
}