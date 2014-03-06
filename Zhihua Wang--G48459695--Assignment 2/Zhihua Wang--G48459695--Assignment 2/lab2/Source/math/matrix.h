#ifndef __GWU_MATRIX__
#define __GWU_MATRIX__

//================================
// matrix
//================================

class Matrix {

public:
	float mat[16];

public:

	Matrix() {
		for(int i=0; i<4; i++) {
			for(int j=0; j<4; j++) {
				if(i==j)
					mat[i+j*4]=1.0;
				else
					mat[i+j*4]=0.0;
			}
		}
	}

	~Matrix() {
	}

	void init() {
		for(int i=0; i<4; i++) {
			for(int j=0; j<4; j++) {
				if(i==j)
					mat[i+j*4]=1.0;
				else
					mat[i+j*4]=0.0;
			}
		}
	}

	//multiply two Matrixes
	void MultiMatrix(Matrix a) {
		float temp[16];
		for(int i=0; i<4; i++) {
			for(int j=0; j<4; j++) {
				temp[4*i+j]=0.0;
				for(int k=0; k<4; k++) {
					temp[i*4+j]+=mat[4*k+j]*a.mat[4*i+k];
				}
			}
		}
		for(int i=0; i<16; i++) {
			mat[i]=temp[i];
		}
	}

	void antiMultiMatrix(Matrix a) {
		float temp[16];
		for(int i=0; i<4; i++) {
			for(int j=0; j<4; j++) {
				temp[4*i+j]=0.0;
				for(int k=0; k<4; k++) {
					temp[i*4+j]+=a.mat[4*k+j]*mat[4*i+k];
				}
			}
		}
		for(int i=0; i<16; i++) {
			mat[i]=temp[i];
		}
	}

	//set the value of a Matrix
	void SetMatrix(Matrix a) {
		for(int i=0; i<16; i++)
			mat[i]=a.mat[i];
	}

	//add two matrixes
	void AddMatrix(Matrix a) {
		for(int i=0; i<16; i++)
			mat[i]+=a.mat[i];
	}


};



#endif //__GWU_MATRIX__