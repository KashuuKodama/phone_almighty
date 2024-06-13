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
#ifdef GAMESCENE_H
#else
#define GAMESCENE_H
#define SAKURANBO_SIZE 0.5f
#define BUDOU_SIZE 0.8f
#define ORANGE_SIZE 1.2f
#define RINGO_SIZE 1.6f
#define MOMO_SIZE 2.0f
#define PINEAPPLE_SIZE 2.5f
#define SUIKA_SIZE 3.3f
#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))
float FruitSize[] = {SAKURANBO_SIZE,BUDOU_SIZE,ORANGE_SIZE,RINGO_SIZE,MOMO_SIZE,PINEAPPLE_SIZE,SUIKA_SIZE};
void Draw3DFruit(Camera* camera,Vector3 FruitPosition,int FruitType){
    if(FruitType==0){
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(SAKURANBO_SIZE,Vec3(1,1,1))),*SakuranboTexture(),1);
        FruitPosition.y=-3.9f;
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(SAKURANBO_SIZE,Vec3(1,0.01f,1))),*ShadowTexture(),1);
    }
    if(FruitType==1){
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(BUDOU_SIZE,Vec3(1,1,1))),*BudouTexture(),1);
        FruitPosition.y=-3.9f;
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(BUDOU_SIZE,Vec3(1,0.01f,1))),*ShadowTexture(),1);
    }
    if(FruitType==2){
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(ORANGE_SIZE,Vec3(1,1,1))),*OrengeTexture(),1);
        FruitPosition.y=-3.9f;
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(ORANGE_SIZE,Vec3(1,0.01f,1))),*ShadowTexture(),1);
    }
    if(FruitType==3){
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(RINGO_SIZE,Vec3(1,1,1))),*RingoTexture(),1);
        FruitPosition.y=-3.9f;
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(RINGO_SIZE,Vec3(1,0.01f,1))),*ShadowTexture(),1);
    }
    if(FruitType==4){
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(MOMO_SIZE,Vec3(1,1,1))),*MomoTexture(),1);
        FruitPosition.y=-3.9f;
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(MOMO_SIZE,Vec3(1,0.01f,1))),*ShadowTexture(),1);
    }
    if(FruitType==5){
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(PINEAPPLE_SIZE,Vec3(1,1,1))),*PineappleTexture(),1);
        FruitPosition.y=-3.9f;
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(PINEAPPLE_SIZE,Vec3(1,0.01f,1))),*ShadowTexture(),1);
    }
    if(FruitType==6){
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(SUIKA_SIZE,Vec3(1,1,1))),*SuikaTexture(),1);
        FruitPosition.y=-3.9f;
        Draw3DModel(camera,*Sphere(),TRS(FruitPosition,Vec3(0,0,0),Vec3_Mul(SUIKA_SIZE,Vec3(1,0.01f,1))),*ShadowTexture(),1);
    }
}
void GameScene(Camera* camera){
    PhysicsSystem3D* physystem=PhysicsSystem();
    camera->pos.z=-10;
    float theta=1.3f;
    float phi=1.57f;
    float r=11;
    Vector3 FruitPosition = Vec3(0,6,0);
    int FruitType = 0;
    int score = 0;

    while (1)
    {
        UpdateTime();
        float time=GetTime();
        BeginCamera(camera);
        camera->euler.y=1.57f-phi;
        camera->pos.y=r*cos(theta);
        camera->pos.z=-r*sin(theta)*sin(phi);
        camera->pos.x=r*sin(theta)*cos(phi);
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
       PhysicsSystem_Update(physystem,FruitSize,&score);
        for(int i=0;i<physystem->objects_length;i++){
            Rigidbody3D* body=PhysicsSystem_GetObject(physystem,i);
            if(body->fruitType==0){
                Draw3DModel(camera,*Sphere(),TRS(body->position,body->position,Vec3_Mul(SAKURANBO_SIZE,Vec3(1,1,1))),*SakuranboTexture(),1);
            }
            if(body->fruitType==1){
                Draw3DModel(camera,*Sphere(),TRS(body->position,body->position,Vec3_Mul(BUDOU_SIZE,Vec3(1,1,1))),*BudouTexture(),1);
            }
            if(body->fruitType==2){
                Draw3DModel(camera,*Sphere(),TRS(body->position,body->position,Vec3_Mul(ORANGE_SIZE,Vec3(1,1,1))),*OrengeTexture(),1);
            }
            if(body->fruitType==3){
                Draw3DModel(camera,*Sphere(),TRS(body->position,body->position,Vec3_Mul(RINGO_SIZE,Vec3(1,1,1))),*RingoTexture(),1);
            }
            if(body->fruitType==4){
                Draw3DModel(camera,*Sphere(),TRS(body->position,body->position,Vec3_Mul(MOMO_SIZE,Vec3(1,1,1))),*MomoTexture(),1);
            }
            if(body->fruitType==5){
                Draw3DModel(camera,*Sphere(),TRS(body->position,body->position,Vec3_Mul(PINEAPPLE_SIZE,Vec3(1,1,1))),*PineappleTexture(),1);
            }
            if(body->fruitType==6){
                Draw3DModel(camera,*Sphere(),TRS(body->position,body->position,Vec3_Mul(SUIKA_SIZE,Vec3(1,1,1))),*SuikaTexture(),1);
            }
        }
        char score_str[64];
        sprintf(score_str,"%d",score);
        Draw2DText(camera,score_str,0.1f,Vec3(0,0,100),Vec2(20,20),255);
        Draw3DQuad(camera,Vec3(-4,-4,-4),Vec3(4,-4,-4),Vec3(4,-4,4),Vec3(-4,-4,4),Vec2(0,0),Vec2(1,0),Vec2(1,1),Vec2(0,1),*FloorTexture(),60);
        Draw3DLine(camera,Vec3(-4,-4,-4),Vec3(-4,20,-4),90);
        Draw3DLine(camera,Vec3(-4,-4,4),Vec3(-4,20,4),90);
        Draw3DLine(camera,Vec3(4,-4,-4),Vec3(4,20,-4),90);
        Draw3DLine(camera,Vec3(4,-4,4),Vec3(4,20,4),90);
        char key=getkey();
        //視点移動
        if(key=='a'){
            phi+=GetDeltaTime()*20;
        }
        if(key=='d'){
            phi-=GetDeltaTime()*20;
        }
        if(key=='s'){
            theta+=GetDeltaTime()*20;
        }
        if(key=='w'){
            theta-=GetDeltaTime()*20;
        }
        if(key=='q'){
            r-=GetDeltaTime()*20;
        }
         if(key=='e'){
            r+=GetDeltaTime()*20;
        }
        //フルーツを落とす場所を移動
        if(key=='j'){
            FruitPosition.x-=100*GetDeltaTime();
        }
        if(key=='l'){
            FruitPosition.x+=100*GetDeltaTime();
        }
        if(key=='i'){
            FruitPosition.z+=100*GetDeltaTime();
        }
        if(key=='k'){
            FruitPosition.z-=100*GetDeltaTime();
        }
        FruitPosition.x=MAX(MIN(FruitPosition.x,4-FruitSize[FruitType]),-4+FruitSize[FruitType]);
        FruitPosition.z=MAX(MIN(FruitPosition.z,4-FruitSize[FruitType]),-4+FruitSize[FruitType]);
        Draw3DFruit(camera,FruitPosition,FruitType);
        //フルーツを落とす
        if(key=='b'){
           
            if(physystem->objects_length==0||FruitSize[PhysicsSystem_GetObject(physystem,physystem->objects_length-1)->fruitType]+FruitSize[FruitType] < sqrt(Vec3_SqrMagnitude(Vec3_LinearSum(1,PhysicsSystem_GetObject(physystem,physystem->objects_length-1)->position,-1,FruitPosition)))){
                 Rigidbody3D* body=Rigidbody();
                body->position=FruitPosition;
                body->old_position=FruitPosition;
                body->fruitType=FruitType;
                PhysicsSystem_Add(physystem,body);
                if(fmod(time,0.1f)<0.04f){
                    FruitType=0;
                }
                else if(fmod(time,0.1f)<0.07f){
                    FruitType=1;
                }
                else{
                    FruitType=2;
                }
            }
        }
        //次の画面に移るときにbreak
        int gameover = 0;
        for(int i=0;i<physystem->objects_length;i++){
            if(physystem->objects[i]->position.y+FruitSize[physystem->objects[i]->fruitType] > 8){
                if(Rigidbody_GetVelocity(physystem->objects[i]).x==0&&Rigidbody_GetVelocity(physystem->objects[i]).y==0&&Rigidbody_GetVelocity(physystem->objects[i]).z==0){
                    gameover = 1;
                }
            }
        }
        if(gameover){
            break;
        }
        EndCamera(camera);
    }
    while(1){
        camera->pos=Vec3(0,0,-1);
        UpdateTime();
        float time=GetTime();
        BeginCamera(camera);
        char score_str[64];
        sprintf(score_str,"%d",score);
        Draw2DText(camera,score_str,0.1f,Vec3(30,-25,100),Vec2(10,10),220);
        Draw2DText(camera,"score:",0.1f,Vec3(-30,-25,100),Vec2(10,10),220);
        Draw3DText(camera,"GAME OVER",0.2f,TRS(Vec3(0,0,8),Vec3(0,0,0),Vec3(1,1,1)),124);
        Draw3DText(camera,fmod(time*4,2)<1?"Press A":"",0.1f,TRS(Vec3(0.5,-5,8),Vec3(0,0,0),Vec3(1,1,1)),255);
        char key=getkey();
        //次の画面に移るときにbreak
        if(key=='a'){
            break;
        }
        EndCamera(camera);
    }
}
#endif