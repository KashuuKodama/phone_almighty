#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "camera.h"
#include "input.h"
#include "primitives.h"
#include "textures.h"
#include "timer.h"
#include "startmenu.h"
#include "phonescene.h"
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
    DBRequests requests;
    //とりあえずゲストユーザー
    db.user_id=0;

    if (argc == 4) {
        GenClient(argv[1],atoi(argv[2]),atoi(argv[3]),&db,&requests);
    }
    if (argc == 3) {
        GenServer(atoi(argv[1]),atoi(argv[2]));
        sleep(3);
        GenClient("0.0.0.0",atoi(argv[1]),atoi(argv[2]),&db,&requests);
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
    {
        //初めの入力は消去
        int tmp[100];
        getkeys(tmp,100);
        StartMenu(camera,&db,&requests);
        goto rooms;
    }
rooms:
    {
            //初めの入力は消去
        int tmp[100];
        getkeys(tmp,100);
        int ret=RoomsScene(camera,&db,&requests);
        if(ret==1){
            goto chat;
        }
        else{
            goto start;
        }
    }
chat:
    {
        //初めの入力は消去
        int tmp[100];
        getkeys(tmp,100);
        int ret=ChatScene(camera,&db,&requests);
        if(ret==1){
            goto call;
        }
        else{
            goto rooms;
        }
    }
call:
    {
        //初めの入力は消去
        int tmp[100];
        getkeys(tmp,100);
        int ret=PhoneScene(camera,&db,&requests);
        if(ret==-1){
            goto chat;
        }
        FreeCamera(camera);
        free(camera);
    }
}