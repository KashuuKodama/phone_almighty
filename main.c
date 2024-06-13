#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "camera.h"
#include "input.h"
#include "primitives.h"
#include "textures.h"
#include "timer.h"
#include "startmenu.h"
#include "chatscene.h"
#define PI 3.14f
#define Deg2Rad PI/180

int main(){

    Camera* camera=SetupCamera(Deg2Rad*3,120,60);
    camera->pos.z=-1;
    //起動画面　
    StartMenu(camera);
    //ゲーム画面
    ChatScene(camera);
    
    FreeCamera(camera);
    free(camera);
}