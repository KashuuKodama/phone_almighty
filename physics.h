#include "vector3.h"
#include "timer.h"
#include<stdlib.h>
#include<math.h>
#ifdef RIGIDBODY_H
#else
#define RIGIDBODY_H
#define MAX_OBJECTS_COUNT 100
#define MIN(i, j) (((i) < (j)) ? (i) : (j))
#define MAX(i, j) (((i) > (j)) ? (i) : (j))
#define MAX_SPEED 0.004
//3次元剛体を定義するヘッダ
typedef struct rigidbody
{
    int fruitType;
    Vector3 position;
    Vector3 rotation;
    Vector3 old_position;
    Vector3 rold_otation;
    int alive;
}Rigidbody3D;
typedef struct physicssystem{
    Rigidbody3D** objects;
    int objects_length;
}PhysicsSystem3D;
Rigidbody3D* Rigidbody(){
    Rigidbody3D* self=(Rigidbody3D*)malloc(sizeof(Rigidbody3D));
    self->alive=1;
    return self;
}
Vector3 Rigidbody_GetVelocity(Rigidbody3D* self){
    Vector3 Velocity=Vec3_Add(self->position,Vec3_Mul(-1,self->old_position));
    float v=sqrt(Vec3_SqrMagnitude(Velocity));
    if(v>MAX_SPEED/GetDeltaTime()){
        Velocity=Vec3_Mul(MAX_SPEED/GetDeltaTime()/v,Velocity);
    }
    return Velocity;
}
void Rigidbody_Update(Rigidbody3D* self){
    Vector3 tmp=self->position;
    self->position=Vec3_Add(self->position,Rigidbody_GetVelocity(self));
    self->old_position=tmp;
}
PhysicsSystem3D* PhysicsSystem(){
    PhysicsSystem3D* self=(PhysicsSystem3D*)malloc(sizeof(PhysicsSystem3D));
    self->objects=(Rigidbody3D**)malloc(sizeof(Rigidbody3D*)*MAX_OBJECTS_COUNT);
    return self;
}
Rigidbody3D* PhysicsSystem_GetObject(PhysicsSystem3D * self,int index){
    return self->objects[index];
}
void PhysicsSystem_Add(PhysicsSystem3D * self,Rigidbody3D* rigidbody){
    self->objects[self->objects_length]=rigidbody;
    self->objects_length++;
}
void PhysicsSystem_Update(PhysicsSystem3D * self,float* fruitsize,int* score){
    const float dk=-0.001f;
    //位置更新
    for(int i=0;i<self->objects_length;i++){
        Rigidbody_Update(self->objects[i]);
        self->objects[i]->position=Vec3_LinearSum(1,self->objects[i]->position,1,Vec3(0,-0.09f*GetDeltaTime(),0));
        self->objects[i]->position=Vec3_LinearSum(1,self->objects[i]->position,dk,Rigidbody_GetVelocity(self->objects[i]));
    }
    const float k=0.5f;
    const int iter=4;
    //拘束反復
    for(int step=0;step<iter;step++){
        for(int i=0;i<self->objects_length;i++){
            for(int j=i+1;j<self->objects_length;j++){
                Rigidbody3D* a=self->objects[j];
                Rigidbody3D* b=self->objects[i];
                if(!a->alive||!b->alive)continue;
                Vector3 vab= Vec3_LinearSum(1,b->position,-1,a->position);
                float distance=sqrt(Vec3_SqrMagnitude(vab));
                if(distance<fruitsize[a->fruitType]+fruitsize[b->fruitType]){
                    //オブジェクトを合体する
                    if(a->fruitType==b->fruitType){
                        (*score) += a->fruitType+1;
                        if(a->fruitType == 6){
                            a->alive=0;
                            b->alive=0;
                        }
                        else{
                            a->alive=0;
                            b->fruitType+=1;
                            b->position.y+=fruitsize[b->fruitType]-fruitsize[a->fruitType];
                            b->old_position.y+=fruitsize[b->fruitType]-fruitsize[a->fruitType];
                            if(b->position.x-fruitsize[b->fruitType]<-4){
                                b->position.x=-4+fruitsize[b->fruitType];
                                b->old_position.x=-4+fruitsize[b->fruitType];
                            }
                            if(b->position.x+fruitsize[b->fruitType]>4){
                                b->position.x=4-fruitsize[b->fruitType];
                                b->old_position.x=4-fruitsize[b->fruitType];
                            }
                            if(b->position.z-fruitsize[b->fruitType]<-4){
                                b->position.z=-4+fruitsize[b->fruitType];
                                b->old_position.z=-4+fruitsize[b->fruitType];
                            }
                            if(b->position.z+fruitsize[b->fruitType]>4){
                                b->position.z=4-fruitsize[b->fruitType];
                                b->old_position.z=4-fruitsize[b->fruitType];
                            }
                        }
                    }
                    else{
                        float a_radius=fruitsize[a->fruitType];
                        float b_radius=fruitsize[b->fruitType];
                        float totalmass=a_radius*a_radius+b_radius*b_radius;
                        a->position=Vec3_LinearSum(1,a->position,(distance-fruitsize[a->fruitType]-fruitsize[b->fruitType])*b_radius*b_radius*k/distance/totalmass,vab);
                        b->position=Vec3_LinearSum(1,b->position,-(distance-fruitsize[a->fruitType]-fruitsize[b->fruitType])*a_radius*a_radius*k/distance/totalmass,vab);
                    }
                }
            }
        }
        //反発係数
        const float e=0.5f;
        for(int i=0;i<self->objects_length;i++){
            Rigidbody3D* a=self->objects[i];
            if(a->position.y-fruitsize[a->fruitType]<-4){
                a->position.y=MAX(a->old_position.y*e-4*(1.0f-e),fruitsize[a->fruitType]-4);
            }
            if(a->position.x-fruitsize[a->fruitType]<-4){
                a->position.x=MAX(a->old_position.x*e-4*(1.0f-e),fruitsize[a->fruitType]-4);
            }
            if(a->position.x+fruitsize[a->fruitType]>4){
                a->position.x=MIN(a->old_position.x*e+4*(1.0f-e),4-fruitsize[a->fruitType]);
            }
             if(a->position.z-fruitsize[a->fruitType]<-4){
                a->position.z=MAX(a->old_position.z*e-4*(1.0f-e),fruitsize[a->fruitType]-4);
            }
            if(a->position.z+fruitsize[a->fruitType]>4){
                a->position.z=MIN(a->old_position.z*e+4*(1.0f-e),4-fruitsize[a->fruitType]);
            }
        }
    }
    int offset=0;
    for(int i=0;i<self->objects_length;i++){
        Rigidbody3D* a=self->objects[i+offset];
        if(!a->alive){
            offset++;
            i--;
            self->objects_length--;
            continue;
        }
        self->objects[i]=a;
    }
}
#endif