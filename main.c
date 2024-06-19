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
    if (argc == 4) {
        GenClient(argv[1],atoi(argv[2]),atoi(argv[3]),&db,&request);
        //test
        db.user_id=0;
    }
    if (argc == 3) {
        GenServer(atoi(argv[1]),atoi(argv[2]));
        sleep(4);
        GenClient("0.0.0.0",atoi(argv[1]),atoi(argv[2]),&db,&request);
        //test
        db.user_id=1;
        (db.statuses+1)->muted=1;
    }
    Camera* camera=SetupCamera(Deg2Rad*3,240,180);
    camera->pos.z=-1;
    // while(1){
    //     // int a[3];
    //     // int n=getkeys(a,3);
    //     // for(int i=0;i<n;i++){
    //     //     printf("%d\n",a[i]);
    //     // }
    // }
    //起動画面　
    StartMenu(camera);
    RoomsScene(camera,&db,&request);
    //ゲーム画面
    ChatScene(camera,&db,&request,0);
    FreeCamera(camera);
    free(camera);
}