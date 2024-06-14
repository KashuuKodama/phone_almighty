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
void StartMenu(Camera* camera){
    camera->pos=vec3(0,4,-1);
    Model3D* miku=open_obj("models/miku.obj");
    Texture2D* icon=open_texture("textures/icon.txt");
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
        draw_3d_model(camera,*miku,trs(vec3(0,-20,20),vec3(0,time,0),vec3(4,4,4)),*gen_colortexture(255),1);
        draw_3d_model(camera,*plane(),trs(vec3(0,0,16),vec3(0,0,0),vec3(12,12,12)),*icon,1);
        draw_3d_text(camera,"LINE",0.1,trs(vec3(0.5,0,10),vec3(0,0,0),vec3(1.5,-1.5,1.5)),100);
        draw_3d_text(camera,fmod(time*4,2)<1?"Press A":"",0.1f,trs(vec3(0.5,-5,8),vec3(0,0,0),vec3(1,-1,1)),255);
        
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