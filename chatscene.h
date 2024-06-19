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
#include "network/dbrequest.h"
#include "network/userdata.h"
#ifdef CHATSCENE_H
#else
#define CHATSCENE_H
#define PI 3.14f
#define Deg2Rad PI/180
#define MAX_MESSAGE_LINE_LENGTH 15
#define MAX_STAMPS_COUNT 30
float textbox_left(Camera* camera,UserData user,char* text,float y){
    static Model3D text_half;
    static Model3D text_plane;
    static Model3D circle;
    static Texture2D* stamps;
    Texture2D* icon=User_Get_Icon(&user);
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
        draw_3d_model(camera,circle,trs(vec3(-4+0.1f,y-height/2-0.3f,5),vec3(Deg2Rad*-90,0,0),vec3(0.6,0.6,0.6)),*icon,1);
        draw_3d_model(camera,text_plane,trs(vec3(-2.5+width/2-0.3f,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(width/2,1,height/2)),stamps[id],1);
        free(icon);
        return height+0.6f;
    }
    //text
    else{
        float width=(float)strlen(text)*0.4f;
        float height=0.8*(((int)strlen(text)/MAX_MESSAGE_LINE_LENGTH)+1);
        draw_3d_model(camera,circle,trs(vec3(-4+0.1f,y-height/2-0.3f,5),vec3(Deg2Rad*-90,0,0),vec3(0.6,0.6,0.6)),*icon,1);
        draw_3d_model(camera,text_half,trs(vec3(-2+0.1f,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(height/2,1,1)),*gen_colortexture(255),1);
        draw_3d_model(camera,text_half,trs(vec3(-2+width-0.1f,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*-90,0),vec3(height/2,1,1)),*gen_colortexture(255),1);
        draw_3d_model(camera,text_plane,trs(vec3(-2+width/2,y-height/2-0.3f,5),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(height/2,1,width/2)),*gen_colortexture(255),1);
        draw_3d_text(camera,text,0,trs(vec3(-2+width/2,y-height/2-0.3f,4.9),vec3(0,0,0),vec3(0.4,0.5,0.5)),16);
        free(icon);
        return height+0.6f;
    }
    free(icon);
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
int ChatScene(Camera* camera,DBData* db,DBRequests* requests){
    camera->pos=vec3(0,0,-1);
    Texture2D* back=open_texture("textures/back.txt");
    Texture2D* icon=Room_Get_Icon(DB_Get_Room(db,db->current_room_id));
    Texture2D* phone=open_texture("textures/phone.txt");
    Model3D* circle=open_obj("models/circle.obj");
    RoomData* room=DB_Get_Room(db,db->current_room_id);
    float offset=0;
    float front=0;
    char input_text[MAX_MESSAGE_SIZE];
    strcpy(input_text,"");
    while (1)
    {
        UpdateTime();
        float time=GetTime();

        int keys[3];
        int n=getkeys(keys,3);

        //alphabet
        if(n==1&&(('a'<=keys[0]&&keys[0]<='z')||('A'<=keys[0]&&keys[0]<='Z')||keys[0]==' '||keys[0]=='?'||('0'<=keys[0]&&keys[0]<='9'))){
            sprintf(input_text,"%s%c",input_text,keys[0]);
        }
        //command
        if(n==1&&keys[0]=='_'){
            sprintf(input_text,"%s%c",input_text,keys[0]);
        }
        //backspace
        if(n==1&&(keys[0]==127||keys[0]==8)){
            int last=strlen(input_text);
            if(last>0){
                input_text[last-1]=0;
            }
        }
        //enter
        if(n==1&&keys[0]==10){
            Room_Add_Message(room,Create_MessageData(db->user_id,input_text));
            Create_Request_Chat(requests,db->user_id,db->current_room_id,input_text);
            offset=front;
            strcpy(input_text,"");

        }
        //arrow
        if(n==3&&keys[0]==27&&keys[1]==91){
            switch(keys[2]){
                case 65:
                    offset-=0.6f;
                break;
                case 67:
                    return 1;
                break;
                case 66:
                    offset+=0.6f;
                break;
                case 68:
                    return -1;
                break;
            }
        }
        BeginCamera(camera);
        draw_3d_model(camera,*circle,trs(vec3(-3.3,5,4),vec3(-Deg2Rad*90,0,0),vec3(0.5,0.5,0.5)),*icon,1);
        draw_3d_text(camera,room->name,0.01,trs(vec3(-1,5,4),vec3(0,0,0),vec3(0.4,0.4,0.4)),232);
        draw_3d_model(camera,*circle,trs(vec3(3.3,5,4),vec3(-Deg2Rad*90,0,0),vec3(0.5,0.5,0.5)),*phone,232);
        draw_3d_model(camera,*plane(),trs(vec3(0,0,10),vec3(0,0,0),vec3(18,27,1)),*back,1);

        float y=offset;
        for(int i=0;i<room->message_length;i++){
            MessageData message=room->messages[i];
            if(message.user_id==db->user_id){
                y-=textbox_right(camera,message.text,y);
            }
            else{
                y-=textbox_left(camera,*(db->registered_users+message.user_id),message.text,y);
            }
        }
        front=offset-y;
        draw_3d_model(camera,*plane(),trs(vec3(0,-5,4),vec3(0,0,0),vec3(9,2,1)),*gen_colortexture(255),1);
        draw_3d_text(camera,">",0,trs(vec3(-3.4,-4.5,3.9),vec3(0,0,0),vec3(0.4,0.5,0.5)),16);
        draw_3d_model(camera,*plane(),trs(vec3(-3+0.4f*strlen(input_text),-4.75,3.9),vec3(0,0,0),vec3(0.4*(fmod(time,1)<0.5?1:0),0.1,0.5)),*gen_colortexture(16),1);
        draw_3d_text(camera,input_text,0,trs(vec3(-3+0.2f*strlen(input_text),-4.5,3.9),vec3(0,0,0),vec3(0.4,0.5,0.5)),16);
        EndCamera(camera);
    }
    
}
#endif