#ifndef __GWU_COLLISION__
#define __GWU_COLLISION__


class Collisionitem {

public:
	vec3 *edgeVert;
	int Num;
	vec3 *verts;
	vec3 *currentVerts;

public:
	
	Collisionitem() {
		Num=0;
		edgeVert=NULL;
		verts=NULL;
		currentVerts=NULL;
	}

	~Collisionitem() {
		if(edgeVert!=NULL)
			delete[] edgeVert;
		if(verts!=NULL)
			delete[] verts;
		if(currentVerts!=NULL)
			delete[] currentVerts;

		Num=0;
		edgeVert=NULL;
		currentVerts=NULL;
		verts=NULL;
	}

	void init(int n) {
		Num=n;
		edgeVert=new vec3[n];
		verts=new vec3[n];
		currentVerts=new vec3[n];
	}

	//calculate axes of every edge
	void initEdge() {
		for(int i=0; i<Num;i++) {
			edgeVert[i]=verts[(i+1)%Num]-verts[i];
		}
	}

	//calculate the current vertex
	void CalCurrentVerts(Matrix a) {
		for(int i=0; i<Num; i++)
			currentVerts[i]=verts[i]*a;
	}


};

class Collision {

public:

	Model *model;
	Collisionitem *collisionItem;
	int numModel;
	vec3 *dir;
	float dirLen;
	vec3 changeVert;

public:

	Collision() {

		numModel=0;
		collisionItem=NULL;
		model=NULL;
		changeVert.x=1.0;
		changeVert.y=0.0;
		changeVert.z=0.0;
	
	}

	~Collision() {
		
		if(collisionItem!=NULL)
			delete[] collisionItem;		

		model=NULL;
		collisionItem=NULL;
		numModel=0;

		changeVert.x=1.0;
		changeVert.y=0.0;
		changeVert.z=0.0;

	}

	//initialize Collision class
	void setCollision(int n, Model *c_model) {

		model=c_model;
		collisionItem=new Collisionitem[n];
		numModel=n;
		
		for(int i=0; i<n; i++) {
			int l=model[i].vexHull.numVerts;
			collisionItem[i].init(l);
			for(int j=0; j<l; j++) {
				collisionItem[i].verts[j]=model[i].vexHull.verts[j];
			}
			collisionItem[i].initEdge();
		}

	}

	//calculate the projection of every point of model id in _axis
	void CalProjection(int id, vec3 _axis, float &Min, float &Max) {
		
		int l=collisionItem[id].Num;
		vec3 temp;
		float _axisLen=_axis.magnitude();
		float tLen;

		Max=Min=(collisionItem[id].currentVerts[0].dot(_axis))/_axisLen;
		
		for(int i=1; i<l; i++) {
			temp=collisionItem[id].currentVerts[i];
			tLen=temp.dot(_axis)/_axisLen;
			if(Max<tLen)
				Max=tLen;
			if(Min>tLen)
				Min=tLen;
		}
	}

	//check if _axis seperate model a and b
	bool checkIfSeperateAxis(int a, int b, vec3 _axis) {
		
		float Mina, Minb, Maxa, Maxb, tLen;

		CalProjection(a, _axis, Mina, Maxa);
		CalProjection(b, _axis, Minb, Maxb);

		if((Mina>Maxb)||(Minb>Maxa))
			return false;

		if((Maxa<=Maxb)&&(Maxa>=Minb)) {
			tLen=Maxa-Minb;
		}
		else {
			tLen=Maxb-Mina;
		}

		if(tLen>dirLen) {
			dirLen=tLen;
			changeVert=_axis;
		}

		return true;

	}

	//check if there is a collision between model a and model a
	bool checkIfCollision(int a, int b) {

		int l;

		//check if every axes of model a seperate two items
		l=collisionItem[a].Num;

		for(int i=0; i<l; i++) {
			vec3 temp;
			temp.x=-collisionItem[a].edgeVert[i].y;
			temp.y=collisionItem[a].edgeVert[i].x;
			temp=temp*model[a].md_mat;
			temp.normalize();
			if(!checkIfSeperateAxis(a, b, temp))
				return false;
		}

		//check if every axes of model b seperate two items
		l=collisionItem[b].Num;

		for(int i=0; i<l; i++) {
			vec3 temp;
			temp.x=-collisionItem[b].edgeVert[i].y;
			temp.y=collisionItem[b].edgeVert[i].x;
			temp=temp*model[b].md_mat;
			temp.normalize();
			if(!checkIfSeperateAxis(a, b, temp))
				return false;
		}

		
		//make sure change direction can seperate two items
		vec3 temp=collisionItem[a].currentVerts[0]-collisionItem[b].currentVerts[0];
		if(temp.dot(changeVert)<0.0) {
			changeVert.x=-changeVert.x;
			changeVert.y=-changeVert.y;
			changeVert.z=-changeVert.z;
		}

		return true;

	}

	//useless
	void update(int id) {
		model[id].Rotate(-acos(model[id].dirVert.x));
		model[id].Rotate(acos( (changeVert.normalize()).x ));
	//	model[id].Translate(vec3(1.0, 0.0, 0.0) );
	}

	bool collision(int id) {

		changeVert.x=1.0;
		changeVert.y=0.0;
		changeVert.z=0.0;
		dirLen=-1.9;

		bool flag=false;

		for(int i=0; i<numModel; i++) {
			collisionItem[i].CalCurrentVerts(model[i].md_mat);
		}

		for(int i=0; i<numModel; i++) {

			if(i==id)
				continue;
			if(checkIfCollision(id, i)) {
		//		update(id);
				flag=true;
			}
		}
		return flag;
	}



};

#endif