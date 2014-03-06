#ifndef __GWU_PARAMETER__
#define __GWU_PARAMETER__

//================================
// global variables
//================================

//number of models
#define MODEL_NUM 4
const float PI=cos(-1.0); 

//frame index
int g_frameIndex;

// the index of the current model controled
int g_mdctrl = 0;

// model
Model *g_model;

//camera of every model
Camera *g_camera;

//global camera
Camera globalCamera;

Matrix *globalProjection;

//path of model
char g_mdpath[MODEL_NUM][20]={"Data/car.d", "Data/shuttle.d", "Data/cow.d"};

//eye, center and up vector of evey camera
vec3 g_cmvector[MODEL_NUM][3]={ {vec3(2.0,2.0,2.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0) }, 
																	{vec3(2.0,2.0,2.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0) }, 
																	{vec3(2.0,2.0,2.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0) } };

//eye, center and up vector of global camera
vec3 globalVector[3]={vec3(2.0,2.0,2.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 1.0, 0.0)};

//color of model
float g_mdcolor[MODEL_NUM][4]={1.0, 1.0, 1.0, 1.0, 
																1.0, 0.0, 0.0, 1.0, 
																1.0, 1.0, 0.0, 1.0/*,
																1.0, 1.0, 0.0, 1.0,
																1.0, 1.0, 0.0, 1.0,
																1.0, 1.0, 0.0, 1.0*/
																};

//mark the index of the currunt modelview matrix is which model
int g_mdid;

//temp for saving modelview matrix 
float temp[16];

//window size
float winWidth=600.0, winHeight=600.0;

//window position
int winXaxis=100, winYaxis=100;

//perspective parameter theta, aspect, dnear, dfar,
float Theta=45.0f, Dnear=0.1f, Dfar=100.0f;

bool curMouseFlag=false;
vec3 curMouse;

char hit[10][200]=
	{"Using 0, 1, 2 can choose one item to control. 0 is to control the car, 1 is to control the shuttle and 2 is to control the cow.\n", 
	"Using w, s, a, d, q, e we can control item¡¯s position.\n",
	"Using up, down arrow we can control car¡¯s scale.\n", 
	"Using left, right arrow and pagedown, pageup we can control car¡¯s rotation.\n",
	"Using j, k, i, I, u, o we can control camera¡¯s position.\n",
	"Using n, m can rotate the camera.\n", 
	"Using mouse we can rotate item along with the mouse move direction around y axis. When click left mouse, item start rotating; when release the click, stop rotating\n"};


#endif