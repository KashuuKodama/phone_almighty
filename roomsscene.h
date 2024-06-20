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
float Room(Camera* camera,Texture2D icon,char* name,float y){
    static Model3D circle;
    if(circle.indices_length==0){
        circle=*open_obj("models/circle.obj");
    }
    draw_3d_model(camera,circle,trs(vec3(-6,y,9.9),vec3(-Deg2Rad*90,0,0),vec3(1.4,1.4,1.4)),icon,1);
    draw_3d_model(camera,circle,trs(vec3(-6,y,9.95),vec3(-Deg2Rad*90,0,0),vec3(1.5,1.5,1.5)),*gen_colortexture(16),1);
    draw_3d_text(camera,name,0.03,trs(vec3(-4+strlen(name)*0.3,y,9.9),vec3(0,0,0),vec3(0.6,0.8,0.8)),232);
    return 3.2;
}
int RoomsScene(Camera* camera,DBData* db,DBRequests* requests){
    camera->pos=vec3(0,0,0);
    Texture2D* searchbar=open_texture("textures/searchbar.txt");
    Model3D* circle=open_obj("models/circle.obj");
    int selection= 0;
    float cursor_pos=0;
    char input_text[MAX_MESSAGE_SIZE];
    strcpy(input_text,DB_Get_Room(db,selection-1)->name);
    db->current_room_id=0;
    int local_room_count=db->rooms_length;
    while (1)
    {
        UpdateTime();
        float time=GetTime();
        int keys[3];
        int n=getkeys(keys,3);
        //arrow
        if(n==3&&keys[0]==27&&keys[1]==91){
            switch(keys[2]){
                case 65:
                    selection--;
                    if(selection>0&&selection<db->rooms_length+1){
                        strcpy(input_text,DB_Get_Room(db,selection-1)->name);
                    }
                break;
                case 67:
                    if(db->current_room_id>=0)
                        return 1;
                break;
                case 66:
                    selection++;
                    if(selection>0&&selection<db->rooms_length+1){
                        strcpy(input_text,DB_Get_Room(db,selection-1)->name);
                    }
                break;
                case 68:
                    return -1;
                break;
            }
        }
        //他ユーザーによるルーム追加
        if(local_room_count!=db->rooms_length){
            local_room_count=db->rooms_length;
            if(selection>0&&selection<db->rooms_length+1){
                strcpy(input_text,DB_Get_Room(db,selection-1)->name);
            }
        }
        BeginCamera(camera);
        draw_3d_model(camera,*plane(),trs(vec3(0,0,10),vec3(0,0,0),vec3(18,27,1)),*gen_colortexture(255),1);
        draw_3d_text(camera,"TALKs",0.03,trs(vec3(0,10.5,9.9),vec3(0,0,0),vec3(1.4,1.4,1.4)),16);
        draw_3d_model(camera,*plane(),trs(vec3(0,8.5,9.8),vec3(0,0,0),vec3(10,2,1)),*searchbar,1);
        if(selection==0){
            cursor_pos=8.5;
        }
        float offset=4+selection>0?selection*3.2:0;
        for(int i=0;i<db->rooms_length;i++){
            RoomData* room=DB_Get_Room(db,i);
            Texture2D* icon=Room_Get_Icon(room);
            if(selection==i+1){
                cursor_pos=offset;
                db->current_room_id=i;
                 //alphabet
                if(n==1&&(('a'<=keys[0]&&keys[0]<='z')||('A'<=keys[0]&&keys[0]<='Z')||keys[0]==' '||keys[0]=='?'||('0'<=keys[0]&&keys[0]<='9'))){
                    sprintf(input_text,"%s%c",input_text,keys[0]);
                    strcpy(room->name,input_text);
                }
                    //backspace
                if(n==1&&(keys[0]==127||keys[0]==8)){
                    int last=strlen(input_text);
                    if(last>0){
                        input_text[last-1]=0;
                    }
                    strcpy(room->name,input_text);
                }
                //enter
                if(n==1&&keys[0]==10){
                    strcpy(room->name,input_text);
                    Create_Request_Edit_Room(requests,i,*room);
                }
                offset-=Room(camera,*icon,input_text,offset);
            }
            else{
                offset-=Room(camera,*icon,room->name,offset);
            }
            free(icon);
        }
        if(selection==db->rooms_length+1){
            cursor_pos=offset;
            if(n==1&&keys[0]==10){
                DB_Add_Room(db,"test",db->rooms_length);
                Create_Request_Add_Room(requests,"test",db->rooms_length);
            }
        }
        selection=(selection+db->rooms_length+2)%(db->rooms_length+2);
        draw_3d_text(camera,"+",0.03,trs(vec3(0,offset,9.8),vec3(0,0,0),vec3(1,1,1)),16);
        draw_3d_text(camera,"<",0.03,trs(vec3(8-0.5,cursor_pos,9.8),vec3(0,0,0),vec3(1,1,1)),16);
        EndCamera(camera);
    }
    
}
#endif