#include "stdafx.h"

// standard
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

// glut
#include <GL/glut.h>

// source
#include <math/matrix.h>
#include <math/vec3.h>
#include <math/operation.h>
#include <math/vexhull.h>
#include <model.h>
#include <collision.h>
#include <parameter/parameter.h>


//================================
// routine for recovering modelview matrix for evey model
//================================
void Recover_Matr(int id) {
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf(g_model[id].md_mat.mat);
}

//================================
// init
//================================
void init() {

	// init something before main loop...

	g_model = new Model[MODEL_NUM];

	// load model
	for(int i=0; i<MODEL_NUM; i++) {
		g_model[i].LoadModel(g_mdpath[i], i);
		g_model[i].setDirection(dirVerts[i]);
		if(i==0) {
			g_model[i].Scale(vec3(0.2, 0.2, 0.2));
			g_model[i].Translate(vec3(0.0, -2.0, 0));
			g_model[i].Rotate(20.0);
		}
		else if(i==1) {
			g_model[i].Scale(vec3(0.3, 0.3, 0.3));
			g_model[i].Translate(vec3(1.0, 1.0, 0));
		}
		else {
			g_model[i].Scale(vec3(0.8, 0.8, 0.8));
			if(i==2)
				g_model[i].Translate(vec3(-1.0, 0.0, 0.0));
			else if(i==3)
				g_model[i].Translate(vec3(0.0, -1.0, 0.0));
			else if(i==4)
				g_model[i].Translate(vec3(1.0, 0.0, 0.0));
			else
				g_model[i].Translate(vec3(0.0, 1.0, 0.0));
		}

		//load collision
		g_collision.setCollision(MODEL_NUM, g_model);

	}

	//set current modelview matrix to that of model 0
	g_mdid=0;

}

//================================
// update every item as the time pass on
//================================
void update( ) {
	
	//only move car, number 0 item
	for(int i=0; i<1; i++) {

		/*When one item has collision with others, 
			it rotates 0.05 and detects collision again. 
			If there is still a collision, it rotates 0.05 again. 
			Repeat this action until there is no collision and the item can move again.*/
		if(g_collision.collision(i)) {
			g_model[i].Rotate(0.05);
			g_model[i].Translate(g_mdvert[i]);
		}
		else
			g_model[i].Translate(g_mdvert[i]);
	}

}

//================================
// render
//================================
void render( void ) {
	// clear color and depth buffer
	glClearColor (0.0, 0.0,0.0, 0.0);
	glClearDepth (1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	// render state
	glEnable(GL_DEPTH_TEST);
	

	// draw model
	for(int i=0; i<MODEL_NUM; i++) {
		glColor4f( g_mdcolor[i][0], g_mdcolor[i][1], g_mdcolor[i][2], g_mdcolor[i][3]);
		Recover_Matr(i);
		g_model[i].DrawEdges2D();
	//	glutSwapBuffers();
		//g_model[i].DrawEdges2DVexhull();
	}

	// swap back and front buffers
	glutSwapBuffers();
}

//================================
// keyboard input
//================================
void key_press( unsigned char key, int x, int y) {

	switch (key) {
	case 'w':		//control item move up
		g_model[g_mdctrl].Translate(vec3(0, 0.1, 0));
		 g_collision.collision(g_mdctrl);
		break;
	case 'a':		//control item move left
		g_model[g_mdctrl].Translate(vec3(-0.1, 0, 0));
		 g_collision.collision(g_mdctrl);
		break;
	case 's':		//control item move down
		g_model[g_mdctrl].Translate(vec3(0, -0.1, 0));
		 g_collision.collision(g_mdctrl);
		break;
	case 'd':		//control item move right
		g_model[g_mdctrl].Translate(vec3(0.1, 0, 0));
		 g_collision.collision(g_mdctrl);
		break;

	//change control item
	case '0':
		g_mdctrl=0;
		break;

	case '1':
		g_mdctrl=1;
		break;
	case '2':
		g_mdctrl=2;
		break;

	/*case '3':
		g_mdctrl=3;
		break;

	case '4':
		g_mdctrl=4;
		break;

	case '5':
		g_mdctrl=5;
		break;

	case '6':
		g_mdctrl=6;
		break;
	
	case '7':
		g_mdctrl=7;
		break;

	case '8':
		g_mdctrl=8;
		break;

	case '9':
		g_mdctrl=9;
		break;*/

	default:
		break;
    }

	// render
	glutPostRedisplay();

}
 
void special_key( GLint key, int x, int y ) {

	switch (key) {
	case GLUT_KEY_RIGHT: //right arrow, rotate control item in counterclockwise
		g_model[g_mdctrl].Rotate(5.0);
		 g_collision.collision(g_mdctrl);
		break;
	case GLUT_KEY_LEFT: //left arrow, rotate control item in clockwise
		g_model[g_mdctrl].Rotate(-5.0);
		 g_collision.collision(g_mdctrl);
		break;
	case GLUT_KEY_DOWN: //down arrow, scale down the control item 
		g_model[g_mdctrl].Scale(vec3(0.95, 0.95, 0.95));
		 g_collision.collision(g_mdctrl);
		break;
	case GLUT_KEY_UP: //up arrow, scale up the control item 
		g_model[g_mdctrl].Scale(vec3(1.05, 1.05, 1.05));
		 g_collision.collision(g_mdctrl);
		break;
	default:      
		break;
	}

	// render
	glutPostRedisplay();

}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape( int w, int h ) {
	// screen size
	winWidth = (float)w;
	winHeight = (float)h;	
	
	// viewport
	glViewport( 0, 0, (GLsizei)w, (GLsizei)h );

	// projection matrix
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
}


//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer(int value ) {
	// increase frame index
	g_frameIndex++;

	update();
	
	// render
	glutPostRedisplay();

	// reset timer
	glutTimerFunc( 16, timer, 0 );
}

//================================
// main
//================================
int main( int argc, char** argv ) {

	printf("%s", hit);

	float PI=acos(-1.0);

	// create opengL window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH );
	glutInitWindowSize( (int)winWidth, (int)winHeight); 
	glutInitWindowPosition( winXaxis, winYaxis);
	glutCreateWindow( argv[0] );

	// init
	init();
	
	// modelview matrix
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	// set callback functions
	glutDisplayFunc( render );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( key_press ); 
	glutSpecialFunc( special_key );
	glutTimerFunc( 16, timer, 0 );
	
	// main loop
	glutMainLoop();

	return 0;
}