#ifndef __GWU_MODEL__
#define __GWU_MODEL__

//================================
// ModelFace
//================================
class ModelFace {
public :
	int		numSides;
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
	vec3*		normalVertex;

	int			numFaces;
	ModelFace *	faces;

	//model's transformation matrix
	Matrix md_mat;

	//model's projection transformation matrix:if we need projection model separately, we use this matrix 
	Matrix pj_mat;

	//model index
	int ID;

public :
	Model() {
		numVerts = 0;
		verts = NULL;

		numFaces = 0;
		faces = NULL;
		md_mat.init();
		pj_mat.init();
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

	void DrawEdges( void ) {
		
		float temp[4];

		for ( int i = 0; i < numFaces; i++ ) {
			
			temp[0]=Random(), temp[1]=Random(), temp[2]=Random(), temp[3]=Random();
			glColor4f( temp[0], temp[1], temp[2], temp[3]);
			glBegin( GL_POLYGON );
			for ( int k = 0; k < faces[ i ].numSides; k++ ) {
				int p0 = faces[ i ].indices[ k ];
				int p1 = faces[ i ].indices[ ( k + 1 ) % faces[ i ].numSides ];
				glNormal3fv( verts[p0].ptr() );
				glVertex3fv( verts[ p0 ].ptr() );
				glNormal3fv( verts[p1].ptr() );
				glVertex3fv( verts[ p1 ].ptr() );
			}
			glEnd();
		}
		//glEnd();
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
	void Scale(vec3 offset) {
		Matrix temp;
		temp.init();
		temp.mat[0]=offset.x;
		temp.mat[5]=offset.y;
		temp.mat[10]=offset.z;
		md_mat.MultiMatrix(temp);
	}

	void Translate( const vec3 &offset ) {
		Matrix temp;
		temp.init();
		temp.mat[12]=offset.x;
		temp.mat[13]=offset.y;
		temp.mat[14]=offset.z;
		md_mat.MultiMatrix(temp);
	}

	void Rotate( float angle, vec3 vector) {
		// rotate ...
		vector.normalize();
		float d=sqrt(vector.y*vector.y+vector.z*vector.z);
		Matrix m1, m2, m3, m4;
		m1.init(), m2.init(), m3.init(), m4.init();
		m4.mat[5]=vector.z/d;
		m4.mat[6]=vector.y/d;
		m4.mat[9]=-vector.y/d;
		m4.mat[10]=vector.z/d;
		m3.mat[0]=d;
		m3.mat[2]=vector.x;
		m3.mat[8]=-vector.x;
		m3.mat[10]=d;
		m1.mat[5]=m4.mat[5];
		m1.mat[6]=-m4.mat[6];
		m1.mat[9]=-m4.mat[9];
		m1.mat[10]=m4.mat[10];
		m2.mat[0]=m3.mat[0];
		m2.mat[2]=-m3.mat[2];
		m2.mat[8]=-m3.mat[8];
		m2.mat[10]=m3.mat[10];
		m3.MultiMatrix(m4);
		m1.MultiMatrix(m2);
		m2.init();
		m2.mat[0]=cos(angle);
		m2.mat[4]=-sin(angle);
		m2.mat[1]=sin(angle);
		m2.mat[5]=cos(angle);
		m1.MultiMatrix(m2);
		m1.MultiMatrix(m3);
		md_mat.MultiMatrix(m1);
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

		// alloc vertex normal, vertex and index buffer
		verts = new vec3[ numVerts ];
		normalVertex = new vec3[numVerts];
		faces = new ModelFace[ numFaces ];
		

		// read vertices
		for ( int i = 0; i < numVerts; i++ ) {
			fscanf( fp, "%f%f%f", &verts[ i ].x, &verts[ i ].y, &verts[ i ].z );
			normalVertex[i].zero();
		}

		// read indices
		for ( int i = 0; i < numFaces; i++ ) {
			ModelFace *face = &faces[ i ];

			fscanf( fp, "%i", &face->numSides );
			faces[ i ].indices = new int[ face->numSides ];

			for ( int k = 0; k < face->numSides; k++ ) {
				fscanf( fp, "%i", &face->indices[ k ] );
			}
			//calculate normal vertexes, the vertex's normal vector is the sum of the normal vector of every faces of a ploygon
			vec3 normal;
			normal=Xproduct( ( verts[ face->indices[ 1 ] ]-verts[ face->indices[ 0 ] ] ),  ( verts[ face->indices[ 2 ] ]-verts[ face->indices[ 1 ] ] ));
			for ( int k = 0; k < face->numSides; k++ ) {
				normalVertex[ face->indices[ k ] ]=normalVertex[ face->indices[ k ] ]+normal;
			}
		}

		//normalize the normal vertexes
		for ( int i = 0; i < numVerts; i++ ) {
			normalVertex[i].normalize();
		}

		// close file
		fclose( fp );

		ResizeModel();

		return true;
	}
};

#endif // __GWU_MODEL__