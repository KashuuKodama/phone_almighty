//スタートメニューのヘッダ
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "camera.h"
#include "input.h"
#include "primitives.h"
#include "textures.h"
#include "timer.h"
#include "math.h"
#include <string.h>
#ifdef ROOMSSCENE_H
#else
#define ROOMSSCENE_H
#define PI 3.14f
#define Deg2Rad PI/180
#define Max_Line_length 10
void Room(Camera* camera,Texture2D icon,RoomData* room,float y){
    static Model3D circle;
    if(circle.indices_length==0){
        circle=*open_obj("models/circle.obj");
    }
    draw_3d_model(camera,circle,trs(vec3(-4.2,y,6),vec3(-Deg2Rad*90,0,0),vec3(1,1,1)),icon,1);
    draw_3d_model(camera,circle,trs(vec3(-4.2,y,6.01),vec3(-Deg2Rad*90,0,0),vec3(1.1,1.1,1.1)),*gen_colortexture(16),1);
    draw_3d_text(camera,room->name,0.03,trs(vec3(-2.2+strlen(room->name)*0.3,y,6),vec3(0,0,0),vec3(0.6,0.8,0.8)),232);
}
void RoomsScene(Camera* camera,DBData* db,DBRequest* request){
    camera->pos=vec3(0,0,-1);
    Texture2D* icon0=open_texture("textures/rooms/icon0.txt");
    Model3D* circle=open_obj("models/circle.obj");
    while (1)
    {
        UpdateTime();
        float time=GetTime();
        BeginCamera(camera);
        for(int i=0;i<db->rooms_length;i++){
            Room(camera,*icon0,DB_Get_Room(db,i),-5*i);
        }

        draw_3d_model(camera,*plane(),trs(vec3(0,0,10),vec3(0,0,0),vec3(18,27,1)),*gen_colortexture(255),1);
        draw_3d_text(camera,"TALKs",0.03,trs(vec3(0,11,9.9),vec3(0,0,0),vec3(1.4,1.4,1.4)),16);
        draw_3d_text(camera,"search",0.03,trs(vec3(0,9,9.9),vec3(0,0,0),vec3(0.8,0.8,0.8)),16);
        char test[7];
        sprintf(test,"%d",db->user_id);
         draw_3d_text(camera,test,0.03,trs(vec3(0,5,9.9),vec3(0,0,0),vec3(0.8,0.8,0.8)),16);
        char key=getkey();
        //次の画面に移るときにbreak
        if(key=='a'){
            break;
        }
        EndCamera(camera);
    }
    
}
#endif