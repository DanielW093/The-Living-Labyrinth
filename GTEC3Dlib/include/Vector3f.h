#pragma once


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

// create new vector3f
vector3f Vector3f( float x, float y, float z);

// vector normalisation
void normalizeVector3f( vector3f* v);

// vector subtraction
vector3f operator-(vector3f a, vector3f b);

// scalar-vector multiplication
vector3f operator*(float f, vector3f b);

// vector division
vector3f operator/(vector3f a, vector3f b);

// vector addition
vector3f operator+(vector3f a, vector3f b);

