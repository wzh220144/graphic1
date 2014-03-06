#include "stdafx.h"

// standard
#include <cassert>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <cstdlib>

// glut
#include <GL/glut.h>

// source
#include <math/matrix.h>
#include <math/vec3.h>
#include <math/operation.h>
#include <math/projection.h>
#include <model.h>
#include <camera.h>
#include <parameter/parameter.h>
#include <helper.h>

//================================
// routine for recovering modelview matrix for evey model
//================================
void Recover_Matr(int id) {
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf(MultiMatrix(globalCamera.cm_mat, g_model[id].md_mat));
}

void Recover_Global() {
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf(globalCamera.cm_mat.mat);
}

//================================
// init
//================================
void init( void ) {
	// init something before main loop...

	srand(time(NULL));

	for(int i=0; i<6;i++) {
		printf("%s", hit[i]);
	}

	g_mdid=0;

	g_model = new Model[MODEL_NUM];
	g_camera = new Camera[MODEL_NUM];
	globalProjection =new Matrix;
	globalProjection->init();

	// load

	
	globalCamera.setLookAt(globalVector[0], globalVector[1], globalVector[2], -1);

	for(int i=0; i<MODEL_NUM;i++) {
		g_model[i].LoadModel( g_mdpath[i], i);
	//	g_camera[i].setLookAt(g_cmvector[i][0], g_cmvector[i][1], g_cmvector[i][2], i);
		switch(i) {
		case 0:
			g_model[i].Scale(vec3(0.3, 0.3, 0.3));
			g_model[i].Translate(vec3(3.0, 1.0, 1.0));
			break;
		case 1:
			g_model[i].Scale(vec3(0.5, 0.5, 0.5));
			g_model[i].Translate(vec3(0.0, 1.0, 2.0));
			break;
		case 2:
			g_model[i].Scale(vec3(0.5, 0.5, 0.5));
			g_model[i].Translate(vec3(0.0, 1.0, 0));
			break;
		default:
			break;
		}
	}

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);

}

//================================
// update
//================================
void update( void ) {
	// do something before rendering...
}

//================================
// render
//================================
void render( void ) {
	// clear color and depth buffer
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glClearDepth ( 1.0 );
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	// enable depth test
	glEnable( GL_DEPTH_TEST );

	// draw grids
	Recover_Global();
	DrawGrids();

	// draw model
	glColor4f( 1.0, 1.0, 1.0, 1.0 );
	for(int i=0; i<MODEL_NUM; i++) {
		//glColor4f( g_mdcolor[i][0], g_mdcolor[i][1], g_mdcolor[i][2], g_mdcolor[i][3]);
		Recover_Matr(i);
		g_model[i].DrawEdges();
	}

	// swap back and front buffers
	glutSwapBuffers();
}

//================================
// mouse input
//================================

void mouse_press(GLint button, GLint action, GLint xMouse, GLint yMouse) {
	
	if( (button==GLUT_LEFT_BUTTON) && (action==GLUT_DOWN)) {
		curMouseFlag=true;
		curMouse.x=(float)xMouse, curMouse.y=(float)yMouse, curMouse.z=0.0;
	}

	else if((button==GLUT_LEFT_BUTTON) && (action==GLUT_UP)) {
		curMouseFlag=false;
	}

}

void mouse_move(GLint xMouse, GLint yMouse) {

	if(curMouseFlag) {
			vec3 a;
			a.x=(float)xMouse, a.y=(float)yMouse, a.z=0.0;
			g_model[g_mdctrl].Rotate(a.dot(curMouse)/a.magnitude()/curMouse.magnitude(), vec3(0.0, 1.0, 0.0));
			curMouse.x=(float)xMouse, curMouse.y=(float)yMouse, curMouse.z=0.0;
	}

}


void key_press( unsigned char key, int x, int y ) {
	switch (key) {
		//item
	case 'w':		//control item move up
		g_model[g_mdctrl].Translate(vec3(0, 0.1, 0));
		break;
	case 'a':		//control item move left
		g_model[g_mdctrl].Translate(vec3(-0.1, 0, 0));
		break;
	case 's':		//control item move down
		g_model[g_mdctrl].Translate(vec3(0, -0.1, 0));
		break;
	case 'd':		//control item move right
		g_model[g_mdctrl].Translate(vec3(0.1, 0, 0));
		break;
	case 'q':	//control item move forward
		g_model[g_mdctrl].Translate(vec3(0, 0, 0.1));
		break;
	case 'e':	//control item move backward
		g_model[g_mdctrl].Translate(vec3(0, 0, -0.1));
		break;

	//camera
	case 'i':		//control camera move up
		globalCamera.Translate(vec3(0, 0.1, 0));
		break;
	case 'j':		//control camera move left
		globalCamera.Translate(vec3(-0.1, 0, 0));
		break;
	case 'k':		//control camera move down
		globalCamera.Translate(vec3(0, -0.1, 0));
		break;
	case 'l':		//control camera move right
		globalCamera.Translate(vec3(0.1, 0, 0));
		break;
	case 'u':	//control camera move forward
		globalCamera.Translate(vec3(0, 0, 0.1));
		break;
	case 'o':	//control camera move backward
		globalCamera.Translate(vec3(0, 0, -0.1));
		break;
	case 'n'://control camera rotate
		globalCamera.Rotate(0.1, vec3(0, 0.1, 0));
		break;
	case 'm'://control camera rotate
		globalCamera.Rotate(-0.1, vec3(0, 0.1, 0));
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

	default:
		return;
    }
	printf("%c\n", key);
}
 
void special_key( int key, int x, int y ) {
	switch (key) {

	case GLUT_KEY_RIGHT: //right arrow, rotate control item with z axis in counterclockwise
		g_model[g_mdctrl].Rotate(1.0, vec3(0, 0, 1.0));
		printf("key_right\n ");
		break;
	case GLUT_KEY_LEFT: //left arrow, rotate control item with z axis in clockwise
		g_model[g_mdctrl].Rotate(-1.0, vec3(0, 0, 1.0));
		printf("key_left\n ");
		break;
	case GLUT_KEY_DOWN: //down arrow, scale down the control item 
		g_model[g_mdctrl].Scale(vec3(0.95, 0.95, 0.95));
		printf("key_down\n ");
		break;
	case GLUT_KEY_UP: //up arrow, scale up the control item 
		g_model[g_mdctrl].Scale(vec3(1.05, 1.05, 1.05));
		printf("key_up\n ");
		break;
	case GLUT_KEY_PAGE_UP:	//Page Up function key, rotate control item with y axis in counterclockwise
		g_model[g_mdctrl].Rotate(1.0, vec3(0, 1.0, 0.0));
		printf("key_pageup\n ");
		break;
	case GLUT_KEY_PAGE_DOWN :	//Page Down function key, rotate control item with y axis in clockwise
		g_model[g_mdctrl].Rotate(-1.0, vec3(0, 1.0, 0.0));
		printf("key_down\n ");
		break;
		break;

	default:      
		break;
	}
}

//================================
// reshape : update viewport and projection matrix when the window is resized
//================================
void reshape( int w, int h ) {
	// screen size
	winWidth  = w;
	winHeight = h;	
	
	glViewport( 0, 0, (GLsizei)winWidth, (GLsizei)winHeight );

	glMatrixMode( GL_PROJECTION );	// projection matrix 

	globalProjection->init();   //perspective projection
	perspective(Theta, (float)winWidth / (float)winHeight, Dnear, Dfar, globalProjection);

	glLoadMatrixf(globalProjection->mat);
}


//================================
// timer : triggered every 16ms ( about 60 frames per second )
//================================
void timer( int value ) {
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
	
	// create opengL window
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH );
	glutInitWindowSize( (int)winWidth, (int)winHeight ); 
	glutInitWindowPosition( winXaxis, winYaxis);
	glutCreateWindow( argv[0] );

	// init
	init();
	
	// set callback functions
	glutDisplayFunc( render );
	glutReshapeFunc( reshape );
	glutKeyboardFunc( key_press ); 
	glutSpecialFunc( special_key );
	glutTimerFunc( 16, timer, 0 );
	glutMouseFunc(mouse_press);
	glutMotionFunc(mouse_move);
	// main loop
	glutMainLoop();

		/*char path[20]="Data/shuttle.d";

		int numVerts, numFaces;
		vec3 *verts;
		ModelFace *	faces;

		// open file
		FILE *fp = fopen( path, "r" );
		FILE *out = fopen("Data/shuttle1.d", "w");
		if ( !fp ) {
			return false;
		}

		// num of vertices and indices
		fscanf( fp, "data%d%d", &numVerts, &numFaces );

		// alloc vertex normal, vertex and index buffer
		verts = new vec3[ numVerts ];
		faces = new ModelFace[ numFaces ];

		for ( int i = 0; i < numVerts; i++ ) {
			fscanf( fp, "%f%f%f", &verts[ i ].x, &verts[ i ].y, &verts[ i ].z );
		}

		// read indices
		for ( int i = 0; i < numFaces; i++ ) {
			ModelFace *face = &faces[ i ];

			fscanf( fp, "%i", &face->numSides );
			faces[ i ].indices = new int[ face->numSides ];

			for ( int k = 0; k < face->numSides; k++ ) {
				fscanf( fp, "%i", &face->indices[ k ] );
			}
		}

		fprintf( out, "data %d %d\n", numVerts, numFaces );

		for ( int i = 0; i < numVerts; i++ ) {
			fprintf(out, "%.7f %.7f %.7f\n", verts[ i ].x, verts[ i ].y, verts[ i ].z );
		}

		for ( int i = 0; i < numFaces; i++ ) {
			ModelFace *face = &faces[ i ];

			fprintf( out, "%d", face->numSides );

			for ( int k = 0; k < face->numSides; k++ ) {
				fprintf( out, " %d", face->indices[ k ]-1 );
			}
			fprintf( out, " \n");
		}

		fclose( out );
		fclose( fp );*/

	return 0;
}