
#ifndef ___VECTOR_H__
#define ___VECTOR_H__


// a single vertex/vector structure
typedef struct
{
   float x;
   float y;
   float z;
} vector3f;

typedef struct
{
	float x;
	float y;
} vector2f;

/*
vector_t operator-(vector_t a, vector_t b);
vector_t operator*(float f, vector_t b);
vector_t operator/(vector_t a, vector_t b);
vector_t operator+(vector_t a, vector_t b);

// vector subtraction
vector3f operator-(vector3f a, vector3f b)
{
	vector3f c;

	c.point[0] = a.point[0] - b.point[0];
	c.point[1] = a.point[1] - b.point[1];
	c.point[2] = a.point[2] - b.point[2];

	return c;
}

// scalar-vector multiplication
vector_t operator*(float f, vector_t b)
{
	vector_t c;

	c.point[0] = f * b.point[0];
	c.point[1] = f * b.point[1];
	c.point[2] = f * b.point[2];

	return c;
}

// vector division
vector_t operator/(vector_t a, vector_t b)
{
	vector_t c;

	c.point[0] = a.point[0] / b.point[0];
	c.point[1] = a.point[1] / b.point[1];
	c.point[2] = a.point[2] / b.point[2];

	return c;
}

// vector addition
vector_t operator+(vector_t a, vector_t b)
{
	vector_t c;

	c.point[0] = a.point[0] + b.point[0];
	c.point[1] = a.point[1] + b.point[1];
	c.point[2] = a.point[2] + b.point[2];

	return c;
}

*/

/*
class CVector  
{
private:
	float x;
	float y;
	float z;

public:
	// Constructor
	CVector(float x, float y, float z)		  { (*this).x=x; (*this).y=y; (*this).z=z; }
	CVector(const CVector& vt)				      { (*this).x=vt.X(); (*this).y=vt.Y(); (*this).z=vt.Z(); }
	CVector()									  { x=0; y=0; z=0; }

	// Destructor
	~CVector()	{ }

	// Getters & Setters
	float X()								{ return x; }
	float Y()								{ return y; }
	float Z()								{ return z; }
	
	float GetX() const					{ return x; }
	float GetY() const					{ return y; }
	float GetZ() const					{ return z; }
	
	void SetX(float x)					{ (*this).x = x; }
	void SetY(float y)					{ (*this).y = y; }
	void SetZ(float z)					{ (*this).z = z; }
	
	void Set(float x, float y, float z)	{ (*this).x=x; (*this).y=y; (*this).z=z;  }
	void Set(const CVector& vt)			{ (*this).x=vt.x; (*this).y=vt.y; (*this).z=vt.z; }
	
	// Distance to vector vt
	float Distance(CVector& vt)
	{
		float x = X() - vt.X(), y = Y() - vt.Y(), z = Z() - vt.Z();
		return (float)sqrt((float)(x*x + y*y + z*z));
	}

	// assignment operators
	CVector& operator =(const CVector& vt)	{ return CVector( vt); }
	CVector& operator +=(const CVector& vt)	{ x+=vt.X(); y+=vt.Y(); z+=vt.Z(); return (*this); }
	CVector& operator -=(const CVector& vt)	{ x-=vt.X(); y-=vt.Y(); z-=vt.Z(); return (*this); }
	CVector& operator *=(float scalar)	{ x*=scalar; y*=scalar; z*=scalar; return (*this); }
	CVector& operator /=(float scalar)	{ x/=scalar; y/=scalar; z/=scalar; return (*this); }

	// unary operators
	CVector operator -() const				{ return CVector(-X(), -Y(), -Z()); } 
	CVector operator +() const				{ return CVector(X(), Y(), Z()); } 

	// binary operators
	CVector operator +(const CVector& vt)		{ CVector res = *this; res += vt; return res; }
	CVector operator -(const CVector& vt)		{ CVector res = *this; res -= vt; return res; }
	CVector operator *(float scalar)		    { CVector res = *this; res *= scalar; return res; }
	CVector operator /(float scalar)	        { CVector res = *this; res /= scalar; return res; }

	// comparisons
	bool operator ==(const CVector& vt)		{ return (X() == vt.X() && Y() == vt.Y() && Z() == vt.Z()); }
	bool operator !=(const CVector& vt)		{ return (X() != vt.X() || Y() != vt.Y()  || Z() != vt.Y()); }
};
* */

#endif
