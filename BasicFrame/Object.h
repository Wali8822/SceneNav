#pragma once

#ifndef __OBJECT__
#define __OBJECT__


#include <math.h>
#include <Windows.h>
#include "Md2Loader.h"
#include "timer.h"


typedef struct
{
	float x,y,z;
}Vector3;

typedef struct  
{
	float x,y;
}Vector2;



enum DIRECTION{RIGHT,LEFT};

class Object
{
public:
	Object(void);
	~Object(void);

	void InitObject(char * modelname,char * skinname);
    void Render();
	void Run();
	void Rotate(DIRECTION d,float ang);
	void Jump();
	void SetState(int s){_loader.SetAnim(s);}
	void Catch(float x,float y,float z);
	void GetPos(float  vert[]);
private:
	float       _posX,_posY,_posZ;
	float       _angle;
	Vector3     _v;
	Md2Loader   _loader;
};

#endif

