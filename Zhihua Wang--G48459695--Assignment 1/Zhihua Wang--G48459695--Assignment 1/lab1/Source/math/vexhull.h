#ifndef __GWU_VEXHULL__
#define __GWU_VEXHULL__

class Vexhull {

public:
	int numVerts;
	vec3 *verts;


public :
	Vexhull() {
		numVerts = 0;
		verts =new vec3[1];
	}

	~Vexhull() {
		if ( verts ) {
			delete[] verts;
			verts = NULL;
		}
		numVerts = 0;
	}

	Vexhull(vec3 *anyPolygon, int num) {
		reconstruct(anyPolygon, num);
	}


	//draw conves hull of the model, used in test program
	void DrawEdges2D() {
		glBegin( GL_LINE_LOOP);
		for ( int i = 0; i < numVerts; i++ ) {
		//	glBegin( GL_LINES);
			glVertex2fv( verts[ i ].ptr() );
			//glVertex2fv( verts[ (i+1)%numVerts ].ptr() );
		//	glEnd();
			//glutSwapBuffers();
		}
		glEnd();
	}


	//calculte convex hull of model a
	void reconstruct(vec3 *a, int num) {
		vec3 anyPolygon[500];
	//	anyPolygon=new vec3[num];
		for(int i=0; i<num; i++)
			anyPolygon[i]=a[i];
		int temp=0;
		verts=new vec3[num];
		numVerts=1;
		for(int i=1; i<num; i++) {
			if( (anyPolygon[i].y <anyPolygon[temp].y) ||( (anyPolygon[i].y ==anyPolygon[temp].y) && (anyPolygon[i].x <anyPolygon[temp].x ) ) )
				temp=i;
		}
		verts[0]=anyPolygon[temp];	
		anyPolygon[temp]=anyPolygon[0];	
		anyPolygon[0]=verts[0];
		origin=anyPolygon[0];
		//sort point in polar coordinate
		quicksort(anyPolygon, 1, num-1);
		//std::sort(anyPolygon+1, anyPolygon+num, cmp);	
		verts[0]=anyPolygon[0];	
		verts[1]=anyPolygon[1];
		for(int i=2; i<num; i++) {
			while( (numVerts>=1) && (Xproduct2d(verts[numVerts-1], verts[numVerts], anyPolygon[i])<=0) ) 
				numVerts--;
			verts[++numVerts]=anyPolygon[i];
		}
		numVerts++;
	}

};

#endif