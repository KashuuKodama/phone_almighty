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
#ifdef CHATSCENE_H
#else
#define CHATSCENE_H
#define PI 3.14f
#define Deg2Rad PI/180
#define MAX_MESSAGE_LINE_LENGTH 15
#define MAX_MESSAGE_LENGTH 30
#define MAX_STAMPS_COUNT 30
float textbox_left(Camera* camera,Texture2D icon,char* text,float y){
    static Model3D text_half;
    static Model3D text_plane;
    static Model3D circle;
    static Texture2D* stamps;
    if(text_half.vertices_length==0){
        text_half=*open_obj("models/circle_half.obj");
    }
    if(text_plane.vertices_length==0){
        text_plane=*open_obj("models/plane.obj");
    }
     if(circle.vertices_length==0){
        circle=*open_obj("models/circle.obj");
    }
    //stamp
    if(text[0]=='_'){
        if(stamps==NULL){
            stamps=malloc(sizeof(Texture2D)*MAX_STAMPS_COUNT);
            for(int i=0;i<MAX_STAMPS_COUNT;i++)
            {
                FILE *file;
                char filename[30];
                sprintf(filename,"textures/stamps/stamp%d.txt",i);
                if ((file = fopen(filename, "r")))
                {
                    fclose(file);
                    stamps[i]=*open_texture(filename);
                    continue;
                }
                break;
            }
        }
        int id=atoi(&text[1]);
        float width=3;
        float height=(float)stamps[id].height/stamps[id].width*width;
        draw_3d_model(camera,circle,trs(vec3(-4+0.1f,y-height/2-0.3f,5),vec3(Deg2Rad*-90,0,0),vec3(0.6,0.6,0.6)),icon,1);
        draw_3d_model(camera,text_plane,trs(vec3(-2.5+width/2-0.3f,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(width/2,1,height/2)),stamps[id],1);
        return height+0.6f;
    }
    //text
    else{
        float width=(float)strlen(text)*0.4f;
        float height=0.8*(((int)strlen(text)/MAX_MESSAGE_LINE_LENGTH)+1);
        draw_3d_model(camera,circle,trs(vec3(-4+0.1f,y-height/2-0.3f,5),vec3(Deg2Rad*-90,0,0),vec3(0.6,0.6,0.6)),icon,1);
        draw_3d_model(camera,text_half,trs(vec3(-2+0.1f,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(height/2,1,1)),*gen_colortexture(255),1);
        draw_3d_model(camera,text_half,trs(vec3(-2+width-0.1f,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*-90,0),vec3(height/2,1,1)),*gen_colortexture(255),1);
        draw_3d_model(camera,text_plane,trs(vec3(-2+width/2,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(height/2,1,width/2)),*gen_colortexture(255),1);
        draw_3d_text(camera,text,0,trs(vec3(-2+width/2,y-height/2-0.3f,4.9),vec3(0,0,0),vec3(0.4,0.5,0.5)),16);
        return height+0.6f;
    }
    return 0;
}
float textbox_right(Camera* camera,char* text,float y){
    static Model3D text_half;
    static Model3D text_plane;
    static Texture2D* stamps;
    if(text_half.vertices_length==0){
        text_half=*open_obj("models/circle_half.obj");
    }
    if(text_plane.vertices_length==0){
        text_plane=*open_obj("models/plane.obj");
    }
     //stamp
    if(text[0]=='_'){
        if(stamps==NULL){
            stamps=malloc(sizeof(Texture2D)*MAX_STAMPS_COUNT);
            for(int i=0;i<MAX_STAMPS_COUNT;i++)
            {
                FILE *file;
                char filename[30];
                sprintf(filename,"textures/stamps/stamp%d.txt",i);
                if ((file = fopen(filename, "r")))
                {
                    fclose(file);
                    stamps[i]=*open_texture(filename);
                    continue;
                }
                break;
            }
        }
        int id=atoi(&text[1]);
        float width=3;
        float height=(float)stamps[id].height/stamps[id].width*width;
        draw_3d_model(camera,text_plane,trs(vec3(4-width/2-0.3f,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(width/2,1,height/2)),stamps[id],1);
        return height+0.6f;
    }
    //text
    else{
        float width=(float)strlen(text)*0.4f;
        float height=0.4*(((int)strlen(text)/MAX_MESSAGE_LINE_LENGTH)+1);
        draw_3d_model(camera,text_half,trs(vec3(3.5-width+0.1f,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(height,1,1)),*gen_colortexture(46),1);
        draw_3d_model(camera,text_half,trs(vec3(3.5-0.1f,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*-90,0),vec3(height,1,1)),*gen_colortexture(46),1);
        draw_3d_model(camera,text_plane,trs(vec3(3.5-width/2,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(height,1,width/2)),*gen_colortexture(46),1);
        draw_3d_text(camera,text,0,trs(vec3(3.5-width/2,y-height/2-0.3f,4.9),vec3(0,0,0),vec3(0.4,0.5,0.5)),16);
        return height+0.6f;
    }
    return 0;
}
void ChatScene(Camera* camera){
    camera->pos=vec3(0,0,-1);
    Texture2D* back=open_texture("textures/back.txt");
    Texture2D* icon=open_texture("textures/rooms/icon0.txt");
    Texture2D* icon1=open_texture("textures/rooms/icon4.txt");
    Texture2D* phone=open_texture("textures/phone.txt");
    Model3D* circle=open_obj("models/circle.obj");
    float offset=0;
    while (1)
    {
        UpdateTime();
        float time=GetTime();

        char message[MAX_MESSAGE_LENGTH];
        int keys[3];
        int n=getkeys(keys,3);

        //alphabet
        if(n==1&&('a'<=keys[0]&&keys[0]<='z')||('A'<=keys[0]&&keys[0]<='Z')||keys[0]==' '){
            sprintf(message,"%s%c",message,keys[0]);
        }
        //backspace
        if(n==1&&(keys[0]==127||keys[0]==8)){
            int last=strlen(message);
            if(last>0){
                message[last-1]=0;
            }
        }
        //enter
        if(n==1&&keys[0]==10){
            
        }
        //arrow
        if(n==3&&keys[0]==27&&keys[1]==91){
            switch(keys[2]){
                case 65:
                    offset+=0.3f;
                break;
                case 67:
                    //sprintf(message,"right");
                break;
                case 66:
                    offset-=0.3f;
                break;
                case 68:
                    //sprintf(message,"left");
                break;
            }
        }
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
        draw_3d_model(camera,*circle,trs(vec3(-3.3,5,4),vec3(-Deg2Rad*90,0,0),vec3(0.5,0.5,0.5)),*icon,1);
        draw_3d_text(camera,"AIZAWA",0.01,trs(vec3(-1,5,4),vec3(0,0,0),vec3(0.4,0.4,0.4)),232);
        draw_3d_model(camera,*circle,trs(vec3(3.3,5,4),vec3(-Deg2Rad*90,0,0),vec3(0.5,0.5,0.5)),*phone,232);
        draw_3d_model(camera,*plane(),trs(vec3(0,0,10),vec3(0,0,0),vec3(18,27,1)),*back,1);
        
        float y=offset;
        y-=textbox_left(camera,*icon1,"Hey hello",y);
        y-=textbox_right(camera,"Hey hello",y);
        y-=textbox_left(camera,*icon1,"yeah hello",y);
        y-=textbox_right(camera,"hello",y);
        y-=textbox_right(camera,"_0",y);
        y-=textbox_left(camera,*icon1,"_3",y);

        draw_3d_model(camera,*plane(),trs(vec3(0,-5,4),vec3(0,0,0),vec3(9,2,1)),*gen_colortexture(255),1);
        draw_3d_text(camera,">",0,trs(vec3(-3.4,-4.5,3.9),vec3(0,0,0),vec3(0.4,0.5,0.5)),16);
        draw_3d_model(camera,*plane(),trs(vec3(-3+0.4f*strlen(message),-4.75,3.9),vec3(0,0,0),vec3(0.4*(fmod(time,1)<0.5?1:0),0.1,0.5)),*gen_colortexture(16),1);
        draw_3d_text(camera,message,0,trs(vec3(-3+0.2f*strlen(message),-4.5,3.9),vec3(0,0,0),vec3(0.4,0.5,0.5)),16);
        EndCamera(camera);
    }
    
}
#endif