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
#include "network/userdata.h"
#include "network/dbdata.h"
#include "network/dbrequest.h"
#ifdef STARTMENU_H
#else
#define STARTMENU_H
#define PI 3.14f
#define Deg2Rad PI/180
void StartMenu(Camera* camera,DBData* db,DBRequest* request){
    camera->pos=vec3(0,0,-1);
    camera->euler=vec3(0,0,0);
    Model3D* miku=open_obj("models/sphere.obj");
    Model3D* circle=open_obj("models/circle.obj");
    Texture2D* earth=open_texture("textures/earth.txt");
    Texture2D* back=open_texture("textures/back.txt");
    Texture2D* side=open_texture("textures/text_side.txt");
    FILE* profile_fd=fopen("profile/profile.txt","r");
    char input_text[MAX_NAME_SIZE];
    char line[30];
    fgets(line,30,profile_fd);
    sscanf(line,"name:%s",input_text);
    char message[30];
    int icon_id=0;
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
        int id=DB_Find_User(db,input_text);
        //未登録
        if(id==-1){
            strcpy(message,"NEW User");
            //enter
            if(n==1&&keys[0]==10){
                Create_Request_Add_User(request,CreateUser(input_text,icon_id));
            }
        }
        //登録済み
        else{
            db->user_id=id;
            strcpy(message,"Registered User");
        }
        //change icon
        if(id>=0&&n==3&&keys[0]==27&&keys[1]==91&&keys[2]==65){
            icon_id++;
            Create_Request_Edit_User(request,db->user_id,CreateUser(input_text,icon_id));
        }
        //skip
        if(id>=0&&n==3&&keys[0]==27&&keys[1]==91&&keys[2]==67){
            free(miku);
            free(circle);
            free(earth);
            free(back);
            free(side);
            fclose(profile_fd);
            break;
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
        //draw_3d_model(camera,*plane(),trs(vec3(0,020),vec3(0,0,0),vec3(10,-10,10)),*gen_alphabettexture('A'),1);
        draw_3d_text(camera,"EEIC",0.1,trs(vec3(0.5,7,9),vec3(0,0,0),vec3(1.5,1.5,1.5)),255);
        draw_3d_text(camera,message,0.1,trs(vec3(0.5,-3,9),vec3(0,0,0),vec3(0.6,0.6,1)),16);
        draw_3d_text(camera,input_text,0.01,trs(vec3(0.5,-2,9),vec3(0,0,0),vec3(0.7,0.7,1)),255);
        draw_3d_text(camera,fmod(time*4,2)<1?"Press right":"",0.1f,trs(vec3(0.5,-9,9),vec3(0,0,0),vec3(1,1,1)),255);
        Texture2D* icon=id==-1?open_texture("textures/icons/icon0.txt"):User_Get_Icon(db->registered_users+db->user_id);
        draw_3d_model(camera,*circle,trs(vec3(0,2,9),vec3(-Deg2Rad*90,0,Deg2Rad*180),vec3(3,3,3)),*icon,1);
        free(icon);
        char id_text[10];
        sprintf(id_text,"ID %d",id);
        draw_3d_text(camera,id_text,0.1f,trs(vec3(0.5,10,8.6),vec3(0,0,0),vec3(0.6,0.6,0.6)),255);
        draw_3d_model(camera,*plane(),trs(vec3(0,0,10),vec3(0,0,0),vec3(18,27,1)),*back,1);
        draw_3d_model(camera,*miku,trs(vec3(0,-6,9),vec3(0,time,0),vec3(2,2,2)),*earth,1);
        //Draw3DModel(camera,*Sphere(),TRS(Vec3(6,2-fmod(time+4,8),12),Vec3(time,0,0),Vec3_Mul(RINGO_SIZE*0.8,Vec3(1,1,1))),*RingoTexture(),1);
        //Draw3DModel(camera,*Sphere(),TRS(Vec3(-8,2-fmod(time+5,8),12),Vec3(time,0,0),Vec3_Mul(SUIKA_SIZE*0.8,Vec3(1,1,1))),*SuikaTexture(),1);

        EndCamera(camera);
    }
    
}
#endif