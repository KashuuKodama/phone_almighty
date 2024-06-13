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
#include "physics.h"
#ifdef CHATSCENE_H
#else
#define CHATSCENE_H
void ChatScene(Camera* camera){
    camera->pos=Vec3(0,0,-1);
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

        char key=getkey();
        //次の画面に移るときにbreak
        if(key=='a'){
            break;
        }
        EndCamera(camera);
    }
    
}
#endif