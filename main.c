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
#include "network/network3.h"
#include "network/dbdata.h"
#define PI 3.14f
#define Deg2Rad PI/180

int main(int argc, char *argv[]){
    signal(SIGPIPE, SIG_IGN);
    int status;
    DBData db;
    if (argc == 3) {
        GenClient(argv[1],atoi(argv[2]),atoi(argv[3]),&status,&db);
    }
    if (argc == 2) {
        GenServer(atoi(argv[1]),atoi(argv[2]));
    }
    Camera* camera=SetupCamera(Deg2Rad*3,240,180);
    camera->pos.z=-1;
    /*
    while(1){
        int a[3];
        int n=getkeys(a,3);
        for(int i=0;i<n;i++){
            printf("%d\n",a[i]);
        }
    }
    */
    //起動画面　
    StartMenu(camera);
    RoomsScene(camera);
    //ゲーム画面
    ChatScene(camera,&db,"kashuu");
    FreeCamera(camera);
    free(camera);
}