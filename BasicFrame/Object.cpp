#include "Object.h"

void Normalize(Vector3 * v)
{
	float l = sqrt(v->x*v->x+v->y*v->y+v->z*v->z);

	if (l<=0)
	{
		return ;
	}
	v->x /= l;
	v->y /= l;
	v->z /= l;
}

void Add(Vector3 v1,Vector3 v2,Vector3 * out)
{
	out->x = v1.x + v2.x;
	out->y = v1.y + v2.y;
	out->z = v1.z + v2.z;
	Normalize(out);
}
void Sub(Vector3 v1,Vector3 v2,Vector3 * out)
{
	out->x = v1.x - v2.x;
	out->y = v1.y - v2.y;
	out->z = v1.z - v2.z;
	Normalize(out);
}

float Multi(Vector3 v1,Vector3 v2)
{
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

void Scalar(Vector3 v1,Vector3 v2,Vector3 * out)
{
	out->x = v1.y*v2.z - v1.z*v2.y;
	out->y = v1.z*v2.x - v1.x*v2.z;
	out->z = v1.x*v2.y - v1.y*v1.x;
	Normalize(out);
}

float CrossAngle(Vector3 v1,Vector3 v2)
{
	float ret;
	float l1,l2;

	Normalize(&v1);
	Normalize(&v2);

	l1 = sqrt(v1.x*v1.x + v1.y*v1.y + v1.z*v1.z);
	l2 = sqrt(v2.x*v2.x + v2.y*v2.y + v2.z*v2.z);

	ret = Multi(v1,v2);

	if (l1*l2 == 0.0)
	{
		ret = 0.0;
	}
	else
	{
		ret = ret/(l1*l2);
	}

	return acos(ret);
}



Object::Object(void)
{
	_v.x=_v.y=0.0f;
	_v.z=-1.0f;

	_angle=0.0f;

	_posX=0.0f;
	_posY=0.0f;
	_posZ=0.0f;
}

Object::~Object(void)
{
}

void Object::InitObject( char * modelname ,char * skinname)
{
	_loader.LoadModel(modelname);
	_loader.LoadSkin(skinname);

	_loader.SetAnim(CROUCH_STAND);
	_loader.ScaleModel(1.5);
}

void Object::Render()
{
	CTimer::GetInstance()->Update();
	float timesc = CTimer::GetInstance()->GetTimeMSec()/1000.0;

	glPushMatrix();

	glTranslatef(_posX,_posY,_posZ);   
	glRotatef(_angle,0.0,1.0,0.0);
	_loader.DrawModel(timesc);

	Vector3 v3;
	v3.x=1.0f;
	v3.y=0.0f;
	v3.z=0.0f;
	float ang = CrossAngle(_v,v3);
	float a = ang*180.0f/3.1415926;
	glPushMatrix();
	glTranslatef(0.0,3.0,0.0);
	glBegin(GL_LINES);
	glVertex3f(_posX,0.0,_posZ);
	glVertex3f(_posX+50.0f*cos(ang),0.0,_posZ+50.0f*sin(ang));
	glEnd();
	glPopMatrix();

	glPopMatrix();
}

void Object::Run()
{
	float ang = (3.1415926)*((180.0-_angle)/180.0);
	_posX+=sin(ang);
	_posZ-=cos(ang);
}

void Object::Rotate( DIRECTION d,float ang )
{
	if (d == LEFT)
	{
		_angle += ang;
	}
	else
	{
		_angle -= ang;
	}

	float ang1 = (3.1415926)*((180.0-_angle)/180.0);
	_v.x -= sin(ang1);
	_v.z += cos(ang1);
}

void Object::Jump()
{

}

void Object::Catch( float x,float y,float z )
{
	Vector3 vpee;
    float   rotAngl=0.0f;

	vpee.x = x;
	vpee.y = 0.0f;
	vpee.z = z;

	rotAngl = CrossAngle(_v,vpee);

	(rotAngl < 0) ? (Rotate(LEFT,rotAngl*180.0f)) : (Rotate(RIGHT,rotAngl*180.0f)); 
}

void Object::GetPos( float  vert[] )
{
	vert[0] = _posX;
	vert[1] = _posY;
	vert[2] = _posZ;
}
