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
#ifdef CHATSCENE_H
#else
#define CHATSCENE_H
#define PI 3.14f
#define Deg2Rad PI/180
#define Max_Line_length 15
void textbox_left(Camera* camera,Texture2D icon,char* text,float y){
    static Model3D text_half;
    static Model3D text_plane;
    static Model3D circle;
    if(text_half.vertices_length==0){
        text_half=*open_obj("models/circle_half.obj");
    }
    if(text_plane.vertices_length==0){
        text_plane=*open_obj("models/plane.obj");
    }
     if(circle.vertices_length==0){
        circle=*open_obj("models/circle.obj");
    }
    float width=(float)strlen(text)/5;
    float height=0.4*(((int)strlen(text)/Max_Line_length)+1);
    draw_3d_model(camera,circle,trs(vec3(-4+0.1f,y,5),vec3(Deg2Rad*-90,0,0),vec3(0.6,0.6,0.6)),icon,1);
    draw_3d_model(camera,text_half,trs(vec3(-2+0.1f,y,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(height,1,1)),*gen_colortexture(255),1);
    draw_3d_model(camera,text_half,trs(vec3(-2+width*2-0.1f,y,5),vec3(Deg2Rad*90,Deg2Rad*-90,0),vec3(height,1,1)),*gen_colortexture(255),1);
    draw_3d_model(camera,text_plane,trs(vec3(-2+width,y,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(height,1,width)),*gen_colortexture(255),1);
    draw_3d_text(camera,text,0,trs(vec3(-2+width,y,4.9),vec3(0,0,0),vec3(0.4,0.5,0.5)),16);
}
void textbox_right(Camera* camera,char* text,float y){
    static Model3D text_half;
    static Model3D text_plane;
    if(text_half.vertices_length==0){
        text_half=*open_obj("models/circle_half.obj");
    }
    if(text_plane.vertices_length==0){
        text_plane=*open_obj("models/plane.obj");
    }
    float width=(float)strlen(text)/5;
    float height=0.4*(((int)strlen(text)/Max_Line_length)+1);
    draw_3d_model(camera,text_half,trs(vec3(3.5-width*2+0.1f,y,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(height,1,1)),*gen_colortexture(46),1);
    draw_3d_model(camera,text_half,trs(vec3(3.5-0.1f,y,5),vec3(Deg2Rad*90,Deg2Rad*-90,0),vec3(height,1,1)),*gen_colortexture(46),1);
    draw_3d_model(camera,text_plane,trs(vec3(3.5-width,y,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(height,1,width)),*gen_colortexture(46),1);
    draw_3d_text(camera,text,0,trs(vec3(3.5-width,y,4.9),vec3(0,0,0),vec3(0.4,0.5,0.5)),16);
}
void ChatScene(Camera* camera){
    camera->pos=vec3(0,0,-1);
    Texture2D* back=open_texture("textures/back.txt");
    Texture2D* icon=open_texture("textures/rooms/icon0.txt");
    Texture2D* icon1=open_texture("textures/rooms/icon4.txt");
    Texture2D* phone=open_texture("textures/phone.txt");
    Model3D* circle=open_obj("models/circle.obj");
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
        //Draw3DText(camera,"SUIKA GAME",0.2f,TRS(Vec3(1,0,8),Vec3(0,0,0),Vec3(1,1,1)),39);

        //Draw3DModel(camera,*Sphere(),TRS(Vec3(7,2-fmod(time,8),12),Vec3(time,0,0),Vec3_Mul(PINEAPPLE_SIZE*0.8,Vec3(1,1,1))),*PineappleTexture(),1);
        //Draw3DModel(camera,*Sphere(),TRS(Vec3(6,2-fmod(time+4,8),12),Vec3(time,0,0),Vec3_Mul(RINGO_SIZE*0.8,Vec3(1,1,1))),*RingoTexture(),1);
        //Draw3DModel(camera,*Sphere(),TRS(Vec3(-8,2-fmod(time+5,8),12),Vec3(time,0,0),Vec3_Mul(SUIKA_SIZE*0.8,Vec3(1,1,1))),*SuikaTexture(),1);
        draw_3d_model(camera,*circle,trs(vec3(-4.7,7.4,6),vec3(-Deg2Rad*90,0,0),vec3(0.7,0.7,0.7)),*icon,1);
        draw_3d_text(camera,"AIZAWA",0.01,trs(vec3(-1.5,7.4,6),vec3(0,0,0),vec3(0.6,0.6,0.6)),232);
        draw_3d_model(camera,*circle,trs(vec3(4.7,7.4,6),vec3(-Deg2Rad*90,0,0),vec3(0.7,0.7,0.7)),*phone,232);
        draw_3d_model(camera,*plane(),trs(vec3(0,0,10),vec3(0,0,0),vec3(18,27,1)),*back,1);
        textbox_left(camera,*icon1,"Hey hello",4);
        textbox_right(camera,"Hey hello",2);
        textbox_left(camera,*icon1,"yeah hello",0);
        textbox_right(camera,"hello",-2);
        char key=getkey();
        //次の画面に移るときにbreak
        if(key=='a'){
            break;
        }
        EndCamera(camera);
    }
    
}
#endif