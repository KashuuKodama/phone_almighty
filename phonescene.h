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
#ifdef  PHONESCENE_H
#else
#define PHONESCENE_H
#define PI 3.14f
#define Deg2Rad PI/180


int PhoneScene(Camera* camera,DBData* db,DBRequests* requests){
    camera->pos=vec3(0,0,-1);
    Texture2D* back=open_texture("textures/back.txt");
    Texture2D* icon=open_texture("textures/rooms/icon0.txt");
    Texture2D* phone=open_texture("textures/phone.txt");
    Texture2D* phone_red=open_texture("textures/phone_red.txt");
    Model3D* circle=open_obj("models/circle.obj");
    float offset=0;
    RoomData* room=DB_Get_Room(db,db->current_room_id);

    //会話に参加する
    Create_Request_Phone(requests,db->current_room_id);
    //参加状態にする
    db->statuses[db->user_id].joined=1;
    db->statuses[db->user_id].muted=0;
    Create_Request_Status(requests,db->user_id,db->statuses[db->user_id]);

    while (1)
    {

        if(db->phone_room_id!=-1&&db->phone_room_id!=db->current_room_id){
            //戻る
            return -1;
        }
        else{
            
        }
        //別のトーク会話が開始された時
        int keys[3];
        int n=getkeys(keys,3);
        //mute/unmute
        if(n==1&&keys[0]=='a'){
            db->statuses[db->user_id].muted=0;
            Create_Request_Status(requests,db->user_id,db->statuses[db->user_id]);
        }
        if(n==1&&keys[0]=='b'){
            db->statuses[db->user_id].muted=1;
            Create_Request_Status(requests,db->user_id,db->statuses[db->user_id]);
        }
        //arrow
        if(n==3&&keys[0]==27&&keys[1]==91){
            switch(keys[2]){
                case 65:
                break;
                case 67:
                break;
                case 66:
                break;
                case 68:
                    db->statuses[db->user_id].joined=0;
                    Create_Request_Status(requests,db->user_id,db->statuses[db->user_id]);
                    return -1;
                break;
            }
        }
        UpdateTime();
        float time=GetTime();
        BeginCamera(camera);
        draw_3d_model(camera,*circle,trs(vec3(-3.3,5,4),vec3(-Deg2Rad*90,0,0),vec3(0.5,0.5,0.5)),*icon,1);
        draw_3d_text(camera,room->name,0.01,trs(vec3(-1,5,4),vec3(0,0,0),vec3(0.4,0.4,0.4)),232);
        for(int i=0,j=0;i<MAX_USER_COUNT;i++){
            if(db->statuses[i].joined){
                UserData* user=db->registered_users+i;
                AudioStatus status=db->statuses[i];
                Texture2D* user_icon=User_Get_Icon(user);
                int x=j%2;
                int y=j/2;
                draw_3d_model(camera,*plane(),trs(vec3(-4.5f+x*9,5-9*y,9.9),vec3(0,0,0),vec3(9,9,9)),*user_icon,1);
                free(user_icon);
                draw_3d_model(camera,*circle,trs(vec3(-4.5f+x*9,1.5-9*y,9.8),vec3(Deg2Rad*90,Deg2Rad*90,0),vec3(0.5,0.5,0.5)),*gen_colortexture(status.muted?9:10),1);
                j++;
            }
        }
        draw_3d_model(camera,*plane(),trs(vec3(0,0,10),vec3(0,0,0),vec3(18,27,1)),*back,1);
        draw_3d_model(camera,*plane(),trs(vec3(0,-10,9.8),vec3(0,0,0),vec3(18,4,1)),*gen_colortexture(255),1);
        draw_3d_model(camera,*plane(),trs(vec3(0,-8,9.7),vec3(0,0,0),vec3(3,3,3)),*phone_red,1);
        EndCamera(camera);
        
    }
    
}
#endif