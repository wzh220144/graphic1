#ifndef __GWU_CAMERA__
#define __GWU_CAMERA__

class Camera {

public:
	Matrix cm_mat;
	int ID;

public:
	Camera() {
		cm_mat.init();
	}

	~Camera() {
	}


	void setLookAt(vec3 eye, vec3 center, vec3 up, int id) {
		
		cm_mat.init();

		ID=id;

		vec3 n=eye-center;
		n.normalize();
		vec3 u=Xproduct(up, n);
		u.normalize();
		vec3 v=Xproduct(n, u);
		Matrix temp;
		temp.init();
		temp.mat[0]=u.x, temp.mat[4]=u.y, temp.mat[8]=u.z, temp.mat[12]=-(u.dot(eye));
		temp.mat[1]=v.x, temp.mat[5]=v.y, temp.mat[9]=v.z, temp.mat[13]=-(v.dot(eye));
		temp.mat[2]=u.x, temp.mat[6]=n.y, temp.mat[10]=n.z, temp.mat[14]=-(n.dot(eye));
		cm_mat.MultiMatrix(temp);

	}

	void Translate( const vec3 &offset ) {
		Matrix temp;
		temp.init();
		temp.mat[12]=offset.x;
		temp.mat[13]=offset.y;
		temp.mat[14]=offset.z;
		cm_mat.MultiMatrix(temp);
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
		cm_mat.MultiMatrix(m1);
	}
	void antiRotate( float angle, vec3 vector) {
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
		m1.MultiMatrix(cm_mat);
		for(int i=0; i<16; i++)
			cm_mat.mat[i]=m1.mat[i];
		
	}

};

#endif  //__GWU_CAMERA