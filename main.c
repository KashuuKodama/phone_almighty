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
#include "network/network.h"
#include "network/dbdata.h"
#define PI 3.14f
#define Deg2Rad PI/180

int main(int argc, char *argv[]){

    if (argc == 3) {
        GenClient(argv[1],atoi(argv[2]));
    }
    if (argc == 2) {
        GenServer(atoi(argv[1]));
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
    DBData db;
    db.rooms->length=4;
    MessageData message;
    strcpy(message.text,"hello");
    strcpy(message.user,"kashuu");
    db.rooms->messages[0]=message;
    strcpy(message.text,"hello ooo");
    strcpy(message.user,"yuri");
    db.rooms->messages[1]=message;
    strcpy(message.text,"_0");
    strcpy(message.user,"kashuu");
    db.rooms->messages[2]=message;
     strcpy(message.text,"_3");
    strcpy(message.user,"yuri");
    db.rooms->messages[3]=message;
    ChatScene(camera,&db,"kashuu");
    FreeCamera(camera);
    free(camera);
}