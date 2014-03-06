#ifndef __GWU_MODEL__
#define __GWU_MODEL__

//================================
// ModelFace
//================================
class ModelFace {
public :
	int numSides;
	int *	indices;

public :
	ModelFace() {
		numSides = 0;
		indices = NULL;
	}

	~ModelFace() {
		if ( indices ) {
			delete[] indices;
			indices = NULL;
		}
		numSides = 0;
	}
};

//================================
// Model
//================================
class Model {
public :
	int			numVerts;
	vec3 *		verts;
	vec3			dirVert;

	int			numFaces;
	ModelFace *	faces;

	//store information for the vexhull of the original model
	Vexhull vexHull;

	Matrix md_mat;

	//model index
	int ID;

public :
	Model() {
		numVerts = 0;
		verts =new vec3[1];

		numFaces = 0;
		faces = NULL;
		md_mat.init();
	}

	~Model() {

		if ( verts ) {
			delete[] verts;
			verts = NULL;
		}
		numVerts = 0;

		if ( faces ) {
			delete[] faces;
			faces = NULL;
		}
		numFaces = 0;
		
	}

	void setDirection(vec3 _dir) {
		dirVert=_dir;
	}

	void DrawEdges( void ) {
		glBegin( GL_LINES );
		for ( int i = 0; i < numFaces; i++ ) {
			for ( int k = 0; k < faces[ i ].numSides; k++ ) {
				int p0 = faces[ i ].indices[ k ];
				int p1 = faces[ i ].indices[ ( k + 1 ) % faces[ i ].numSides ];
				glVertex3fv( verts[ p0 ].ptr() );
				glVertex3fv( verts[ p1 ].ptr() );
			}
		}
		glEnd();
	}

	void DrawEdges2D( void ) {
		glBegin( GL_LINES );
		for ( int i = 0; i < numFaces; i++ ) {
			for ( int k = 0; k < faces[ i ].numSides; k++ ) {
				int p0 = faces[ i ].indices[ k ];
				int p1 = faces[ i ].indices[ ( k + 1 ) % faces[ i ].numSides ];
				glVertex2fv( verts[ p0 ].ptr() );
				glVertex2fv( verts[ p1 ].ptr() );
			}
		}
		glEnd();
	}

	void DrawEdges2DVexhull() {
		vexHull.DrawEdges2D();
	}

	// calculate AABB
	bool Bound( vec3 &min, vec3 &max ) {
		if ( numVerts <= 0 ) {
			return false;
		}

		min = verts[ 0 ];
		max = verts[ 0 ];

		for ( int i = 1; i < numVerts; i++ ) {
			vec3 v = verts[ i ];

			if ( v.x < min.x ) {
				min.x = v.x;
			} else if ( v.x > max.x ) {
				max.x = v.x;
			}

			if ( v.y < min.y ) {
				min.y = v.y;
			} else if ( v.y > max.y ) {
				max.y = v.y;
			}

			if ( v.z < min.z ) {
				min.z = v.z;
			} else if ( v.z > max.z ) {
				max.z = v.z;
			}
		}

		return true;
	}

	// scale the model into the range of [ -1, 1 ]
	void ResizeModel( void ) {
		// bound
		vec3 min, max;
		if ( !Bound( min, max ) ) {
			return;
		}

		// center
		vec3 center = ( min + max ) * 0.5f;

		// scale factor
		vec3 size = ( max - min ) * 0.5f;

		float r = size.x;
		if ( size.y > r ) {
			r = size.y;
		}
		if ( size.z > r ) {
			r = size.z;
		}

		if ( r < 1e-6f ) {
			r = 0;
		} else {
			r = 1.0f / r;
		}

		// scale to [ -1, 1 ]
		for ( int i = 0; i < numVerts; i++ ) {
			verts[ i ] -= center;
			verts[ i ] *= r;
		}
	}

	// scale model
	void Scale( const vec3 offset) {
		Matrix a;
		a.init();
		a.mat[0]=offset.x;
		a.mat[5]=offset.y;
		a.mat[10]=offset.z;
		md_mat.MultiMatrix(a);
	}

	//translate model
	void Translate( const vec3 &offset ) {
		Matrix a;
		a.init();
		a.mat[12]=offset.x;
		a.mat[13]=offset.y;
		a.mat[14]=offset.z;
		md_mat.MultiMatrix(a);
	}

	//rotate model
	void Rotate( float angle ) {
		Matrix a;
		a.init();
		a.mat[0]=cos(angle);
		a.mat[5]=cos(angle);
		a.mat[1]=sin(angle);
		a.mat[4]=-sin(angle);
		md_mat.MultiMatrix(a);
		dirVert=dirVert*a;
	/*	Matrix b;
		b.init();
		b.mat[0]=b.mat[5]=dirVert.y;
		b.mat[1]=dirVert.x;
		b.mat[4]=-dirVert.x;
		a.MultiMatrix(b);
		dirVert.x=a.mat[0];
		dirVert.y=a.mat[1];*/
	}

	// translate first then transform the original model
	void antiTranslate( const vec3 &offset ) {
		Matrix a;
		a.init();
		a.mat[12]=offset.x;
		a.mat[13]=offset.y;
		a.mat[14]=offset.z;
		md_mat.antiMultiMatrix(a);
	}

	// rotate first then transform the original model
	void antiRotate( float angle) {
		Matrix a;
		a.init();
		a.mat[0]=cos(angle);
		a.mat[5]=cos(angle);
		a.mat[1]=sin(angle);
		a.mat[4]=-sin(angle);
		md_mat.antiMultiMatrix(a);
		dirVert=dirVert*a;
	/*	Matrix b;
		b.init();
		b.mat[0]=b.mat[5]=dirVert.y;
		b.mat[1]=dirVert.x;
		b.mat[4]=-dirVert.x;
		a.MultiMatrix(b);
		dirVert.x=a.mat[0];
		dirVert.y=a.mat[1];*/
	}

	// load model from .d file
	bool LoadModel( const char *path, int id) {

		ID=id;

		if ( !path ) {
			return false;
		}

		// open file
		FILE *fp = fopen( path, "r" );
		if ( !fp ) {
			return false;
		}

		// num of vertices and indices
		fscanf( fp, "data%d%d", &numVerts, &numFaces );

		// alloc vertex and index buffer
		verts = new vec3[ numVerts ];
		faces = new ModelFace[ numFaces ];

		//used to reconstruct the polygon to convex hull

		// read vertices
		for ( int i = 0; i < numVerts; i++ ) {
			fscanf( fp, "%f%f%f", &verts[ i ].x, &verts[ i ].y, &verts[ i ].z );
			verts[i].z=0.0;
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

		// close file
		fclose( fp );

		ResizeModel();

		vexHull.reconstruct(verts, numVerts);

		return true;
	}
};

#endif // __GWU_MODEL__