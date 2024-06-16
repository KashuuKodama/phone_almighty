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
void Room(Camera* camera,Texture2D icon,char* name,float y){
    static Model3D circle;
    if(circle.indices_length==0){
        circle=*open_obj("models/circle.obj");
    }
    draw_3d_model(camera,circle,trs(vec3(-4.2,y,6),vec3(-Deg2Rad*90,0,0),vec3(1,1,1)),icon,1);
    draw_3d_model(camera,circle,trs(vec3(-4.2,y,6.01),vec3(-Deg2Rad*90,0,0),vec3(1.1,1.1,1.1)),*gen_colortexture(16),1);
    draw_3d_text(camera,name,0.03,trs(vec3(-2.2+strlen(name)*0.3,y,6),vec3(0,0,0),vec3(0.6,0.8,0.8)),232);
}
void RoomsScene(Camera* camera){
    camera->pos=vec3(0,0,-1);
    Texture2D* icon0=open_texture("textures/rooms/icon0.txt");
    Texture2D* icon1=open_texture("textures/rooms/icon1.txt");
    Texture2D* icon2=open_texture("textures/rooms/icon2.txt");
    Texture2D* icon3=open_texture("textures/rooms/icon3.txt");
    Texture2D* icon4=open_texture("textures/rooms/icon4.txt");
    Model3D* circle=open_obj("models/circle.obj");
    while (1)
    {
        UpdateTime();
        float time=GetTime();
        BeginCamera(camera);
        Room(camera,*icon0,"EEIC WHITE",0);
        Room(camera,*icon2,"EEIC BLACK",-2.5);
        Room(camera,*icon1,"UTOKYO",2.5);
        Room(camera,*icon3,"KASHUU",5);
        Room(camera,*icon4,"YURI",-5);
        draw_3d_model(camera,*plane(),trs(vec3(0,0,10),vec3(0,0,0),vec3(18,27,1)),*gen_colortexture(255),1);
        draw_3d_text(camera,"TALK",0.03,trs(vec3(0,11,9.9),vec3(0,0,0),vec3(1.4,1.4,1.4)),16);
        char key=getkey();
        //次の画面に移るときにbreak
        if(key=='a'){
            break;
        }
        EndCamera(camera);
    }
    
}
#endif