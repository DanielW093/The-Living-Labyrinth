#include "Vector3f.h"
#include "math.h"



vector3f Vector3f( float x, float y, float z)
{
	vector3f v;
	v.x=x; v.y=y; v.z=z;
	return v;
}



void normalizeVector3f( vector3f* v)
{
	float sum=(float)sqrt(v->x*v->x+v->y*v->y+v->z*v->z);
	
	if (sum != 0.0f)
	{
		v->x=v->x/sum;
		v->y=v->y/sum;
		v->z=v->z/sum;
	}
}


// vector subtraction
vector3f operator-(vector3f a, vector3f b)
{
	vector3f c;

	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;

	return c;
}

// scalar-vector multiplication
vector3f operator*(float f, vector3f b)
{
	vector3f c;

	c.x = f * b.x;
	c.x = f * b.y;
	c.x = f * b.y;

	return c;
}

// vector division
vector3f operator/(vector3f a, vector3f b)
{
	vector3f c;

	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;

	return c;
}

// vector addition
vector3f operator+(vector3f a, vector3f b)
{
	vector3f c;

	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;

	return c;
}

