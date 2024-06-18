//スタートメニューのヘッダ
#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "camera.h"
#include "input.h"
#include "primitives.h"
#include "textures.h"
#include "timer.h"
#include "network/roomdata.h"
#include "network/messagedata.h"
#include "math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "network/dbdata.h"
#ifdef CALLSCENE_H
#else
#define CALLSCENE_H
#define PI 3.14f
#define Deg2Rad PI/180


void CallScene(Camera* camera,DBData* db,char* user){
    camera->pos=vec3(0,0,-1);
    Texture2D* back=open_texture("textures/back.txt");
    Texture2D* icon=open_texture("textures/rooms/icon0.txt");
    Texture2D* icon1=open_texture("textures/rooms/icon4.txt");
    Texture2D* phone=open_texture("textures/phone.txt");
    Texture2D* phone_red=open_texture("textures/phone_red.txt");
    Model3D* circle=open_obj("models/circle.obj");
    float offset=0;
    while (1)
    {
        UpdateTime();
        float time=GetTime();
        int keys[3];
        int n=getkeys(keys,3);
        if(n==1&&keys[0]==127){
            break;
        }
        BeginCamera(camera);
        draw_3d_model(camera,*circle,trs(vec3(-3.3,5,4),vec3(-Deg2Rad*90,0,0),vec3(0.5,0.5,0.5)),*icon,1);
        draw_3d_text(camera,"AIZAWA",0.01,trs(vec3(-1,5,4),vec3(0,0,0),vec3(0.4,0.4,0.4)),232);
        for(int i=0;i<4;i++){
            for(int j=0;j<2;j++){
                draw_3d_model(camera,*plane(),trs(vec3(-6.75f+i*4.5f,5-4.5*j,9.9),vec3(0,0,0),vec3(4.5,4.5,4.5)),*icon,1);
                draw_3d_text(camera,"AIZAWA",0.01,trs(vec3(-6.75f+i*4.5f,4-4.5*j,9.8),vec3(0,0,0),vec3(0.6,0.6,0.6)),232);
            }
        }
        draw_3d_model(camera,*plane(),trs(vec3(0,0,10),vec3(0,0,0),vec3(18,27,1)),*back,1);
        draw_3d_model(camera,*plane(),trs(vec3(0,-10,9.8),vec3(0,0,0),vec3(18,4,1)),*gen_colortexture(255),1);
         draw_3d_model(camera,*plane(),trs(vec3(0,-8,9.7),vec3(0,0,0),vec3(3,3,3)),*phone_red,1);
        EndCamera(camera);
        
    }
    
}
#endif