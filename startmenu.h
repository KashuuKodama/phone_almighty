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
#ifdef STARTMENU_H
#else
#define STARTMENU_H
#define PI 3.14f
#define Deg2Rad PI/180
void StartMenu(Camera* camera){
    camera->pos=vec3(0,0,-1);
    camera->euler=vec3(0,0,0);
    Model3D* miku=open_obj("models/sphere.obj");
    Model3D* circle=open_obj("models/circle.obj");
    Texture2D* earth=open_texture("textures/earth.txt");
    Texture2D* back=open_texture("textures/back.txt");
    Texture2D* side=open_texture("textures/text_side.txt");
    FILE* profile_fd=fopen("profile/profile.txt","r");
    Texture2D* icon=open_texture("profile/icon.txt");
    char line[30];
    char name[30];
    fgets(line,30,profile_fd);
    sscanf(line,"name:%s",name);
    while (1)
    {
        UpdateTime();
        float time=GetTime();
        BeginCamera(camera);
        //起動画面の描画処理をここに書く
        //つかえる関数一覧
        /*
        Draw2DQuad
        Draw2DTexture
        Draw2DLine
        Draw3DLine
        Draw3DQuad
        Draw3DModel
        Draw3DText
        */
        //Draw3DText(camera,"3D",0.2f,TRS(Vec3(0,-1,3),Vec3(0,time,0),Vec3(1,1,1)),5);
        //draw_3d_model(camera,*plane(),trs(vec3(0,0,20),vec3(0,0,0),vec3(10,-10,10)),*gen_alphabettexture('A'),1);
        draw_3d_text(camera,"EEIC",0.1,trs(vec3(0.5,7,9),vec3(0,0,0),vec3(1.5,1.5,1.5)),255);
        draw_3d_text(camera,name,0.01,trs(vec3(0.5,-2,9),vec3(0,0,0),vec3(0.7,0.7,1)),255);
        draw_3d_text(camera,fmod(time*4,2)<1?"Press A":"",0.1f,trs(vec3(0.5,-9,9),vec3(0,0,0),vec3(1,1,1)),255);
        draw_3d_model(camera,*circle,trs(vec3(0,2,9),vec3(-Deg2Rad*90,0,Deg2Rad*180),vec3(3,3,3)),*icon,1);
        draw_3d_model(camera,*plane(),trs(vec3(0,0,10),vec3(0,0,0),vec3(18,27,1)),*back,1);
        draw_3d_model(camera,*miku,trs(vec3(0,-6,9),vec3(0,time,0),vec3(2,2,2)),*earth,1);
        //Draw3DModel(camera,*Sphere(),TRS(Vec3(6,2-fmod(time+4,8),12),Vec3(time,0,0),Vec3_Mul(RINGO_SIZE*0.8,Vec3(1,1,1))),*RingoTexture(),1);
        //Draw3DModel(camera,*Sphere(),TRS(Vec3(-8,2-fmod(time+5,8),12),Vec3(time,0,0),Vec3_Mul(SUIKA_SIZE*0.8,Vec3(1,1,1))),*SuikaTexture(),1);

        char key=getkey();
        //次の画面に移るときにbreak
        if(key=='a'){
            break;
        }
        EndCamera(camera);
    }
    
}
#endif