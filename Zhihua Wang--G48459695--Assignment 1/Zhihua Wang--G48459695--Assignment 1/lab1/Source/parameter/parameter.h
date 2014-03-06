#ifndef __GWU_PARAMETER__
#define __GWU_PARAMETER__

//================================
// global variables
//================================

//number of models
#define MODEL_NUM 6
const float PI=cos(-1.0); 

//frame index
int g_frameIndex;

// the index of the current model controled
int g_mdctrl = 0;

//collision
Collision g_collision;

// model
Model *g_model;

//path of model
char g_mdpath[MODEL_NUM][20]={"Data/car.d2", "Data/shuttle.d", "Data/line1.d2", "Data/line2.d2", "Data/line3.d2", "Data/line4.d2"};

//color of model
float g_mdcolor[MODEL_NUM][4]={1.0, 1.0, 1.0, 1.0, 
																1.0, 0.0, 0.0, 1.0, 
																1.0, 1.0, 0.0, 1.0,
																1.0, 1.0, 0.0, 1.0,
																1.0, 1.0, 0.0, 1.0,
																1.0, 1.0, 0.0, 1.0
																};

//mark the index of the currunt modelview matrix is which model
int g_mdid;

//temp for saving modelview matrix 
float temp[16];

//information about input stored in char
char hit[200]="up: w       down:d       right: d       left:a\npress different number keys to change different control items\n";

//window sizw
float winWidth=600.0, winHeight=600.0;

//window position
int winXaxis=100, winYaxis=100;

//direction of each item
vec3 dirVerts[MODEL_NUM]={vec3(1.0, 0.0, 0.0), vec3(-1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(1.0, 0.0, 0.0)};

//the direction of erevy item in the world
vec3 g_mdvert[MODEL_NUM]={vec3(0.01, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0)};


#endif