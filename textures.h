//基本テクスチャを定義するヘッダ
#include "texture2d.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef TEXTURES_H
#else
#define TEXTURES_H
Texture2D* gen_colortexture(int color){
    static Texture2D* texture;
    if(texture==NULL){
        texture=gen_texture(1,1);
    }
    texture_set_pixel(texture,0,0,color);
    return texture;
}
Texture2D* open_texture(char* path){
    FILE* fp=fopen(path,"r");
    if(fp==NULL){
        printf("cantopen\n");

        return gen_colortexture(0);
    }
    int x=0;
    int y=0;
    fscanf(fp,"x=%d y=%d",&x,&y);
    Texture2D* texture=gen_texture(y,x);
    for(int i=0;i<y;i++){
        char line[1000];
        fscanf(fp,"%s",line);
        char* p=strtok(line,",");
        for(int j=0;j<x;j++){
            if(p!=NULL){
                char *e;
                long v = strtol(p,&e, 10);
                texture_set_pixel(texture,y-1-i,j,v);
            }
            p=strtok(NULL,",");
        }
    }
    fclose(fp);
    return texture;
}
Texture2D* gen_alphabettexture(char c){
    char path[100]="font/ .txt";
    if(c==' '){
        sprintf(path,"font/ .txt");
    }
    if('a'<=c && c<='z'){
        sprintf(path,"font/%c.txt",c);
    }
    if('A'<=c && c<='Z'){
        sprintf(path,"font/%c.txt",c-'A'+'a');
    }
    if('0'<=c && c<='9'){
        sprintf(path,"font/%c.txt",c);
    }
    return open_texture(path);
}
#endif