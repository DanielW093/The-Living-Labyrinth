#pragma once

#include "GTEC.h"
#include "Color.h"
#include "Texture.h"
#include "Vector3f.h"

using namespace std;


class CModel
{
public:
	
	// ----- Constructors & Destructor -----
	
CModel() 
	{
    Position.x=Position.y=Position.z=0;
    minx=maxx=miny=maxy=minz=maxz=0;
	
	Scale=1; 
	ScaleBB=1;

	TextureID=0; 
	
	Color.Set( 0, 1.0, 0.0, 1.0); // green
	filled=true;
	
	// Time
	SpriteTime=0;
	
	Health=100;	Status=0;		
	
	// Linear Motion { Veclocity and Direction }
	Direction.x=Direction.y=Direction.z=0; // normalized direction vector
	Velocity=0;		 					   // velocity
	   
	// Rotation
	Rotation.x=Rotation.y=Rotation.z=0;	// rotation angle	
	Omega.x=Omega.y=Omega.z=0; // rotation speed

	marked_for_removal=false;  childNode=NULL;
	
	vertexArray=NULL;			
	normalArray=NULL;			
	texArray=NULL;			
	
	numVertices=0;	currentFrame=0;	 maxFrames=0;		
}
	
	
CModel(float x, float y, float z, float width, float height, float depth, const CColor& color=CColor::Blue())
{
	Position.x=x; Position.y=y; Position.z=z;
	minx=-width/2; maxx=width/2; miny=-height/2; maxy=height/2; minz=-depth/2; maxz=depth/2; Scale=1;
		
	ScaleBB=1;

	TextureID=0; 
	
	Color.Set(color);
	filled=true;
	
	// Time
	SpriteTime=0;
	
	Health=100;	Status=0;		
	
	// Linear Motion { Veclocity and Direction }
	Direction.x=Direction.y=Direction.z=0; // normalized direction vector
	Velocity=0;		 					   // velocity
	   
	// Rotation
	Rotation.x=Rotation.y=Rotation.z=0;	// rotation angle	
	Omega.x=Omega.y=Omega.z=0; // rotation speed

	marked_for_removal=false;  childNode=NULL;
	
	vertexArray=NULL;			
	normalArray=NULL;			
	texArray=NULL;			
	
	numVertices=0;	currentFrame=0;			
}
	
	
	virtual ~CModel() 
	{
		
	  if (vertexArray) delete [] vertexArray;
	  if (normalArray) delete [] normalArray;
	  if (texArray) delete [] texArray;	
		
	}

	// ----- Attributes ------

protected:

    // Appearance
	CColor Color;			// model colour
	GLuint TextureID;  		// texture ID, zero if no texture assigned
	bool looping;
	bool filled;		 
	
	// Position & Size
	vector3f Position;		// position (x,y,z) is the center point of the model 
	float minx, maxx, miny, maxy, minz, maxz;
	
	float Scale;			// scale 1=100%
	float ScaleBB;			// scale bounding box

	// Model Time
	Uint32 SpriteTime;		// reference time
	
	// Model states
	int Status;				// user defined status
	int Health;				// health
	bool marked_for_removal;
	
	CModel* childNode;          // child node model reference
	
	
	// Dynamics - Linear
	vector3f Direction;			// movement direction vector
	float Velocity;			// movement velocity in pixel/second
	
	// Dynamics - Rotational
	vector3f Rotation;			// rotation (orientation) in degrees around x,y,z axis
	vector3f Omega;				// rotational velocity in degrees per second per x,y,z axis
	
	vector3f* vertexArray;			// array containing vertex positions
	vector3f* normalArray;			// array containing vertex normals
	vector2f* texArray;			// array containing vertex texture coordinates
	
	int numVertices;				// number of vertices in the model
	int currentFrame, maxFrames;   // current and max frame in animation


public:
	
	// Position functions
	float GetX()								{ return Position.x; }
	float GetY()								{ return Position.y; }
	float GetZ()								{ return Position.z; }
	void SetX( float x)                      { Position.x = x; }
	void SetY( float y)                      { Position.y = y; }
	void SetZ( float z)                      { Position.z = z; }
	void SetToFloor( float y) 			   { Position.y = 2+Scale*abs(int(miny));}    
	void SetPosition(float x, float y,float z)	{ Position.x=x; Position.y=y; Position.z=z;}
	void SetPosition(float x, float z) { Position.x=x; Position.z=z;}
	void SetPositionV(vector3f& pos) { Position.x=pos.x; Position.y=pos.y; Position.z=pos.z;}
	vector3f& GetPositionV() { return Position; }
	
	
	
	void Move(float dx, float dy, float dz)		{ Position.x += dx; Position.y += dy; Position.z += dz;}
	
	//void Move( float distance) {  Position.x += distance*Direction.x; Position.y += distance*Direction.y; Position.z += distance*Direction.z;  }
    void Move( float distance);
    
    void MoveXZRotation( float distance);
	
	
	// Size functions
	void SetSize(float width, float height, float depth) { minx=-width/2; maxx=width/2; miny=-height/2; maxy=height/2; minz=-depth/2; maxz=depth/2; Scale=1; }
	float GetTop() { return Position.y+Scale*maxy;}
	float GetBottom() { return Position.y+Scale*miny; }
	void SetScale(float scale) 		{  Scale=scale; }
	float GetScale()					{ return Scale; }
	void SetScaleBoundingBox( float scale) { ScaleBB=scale; }	
	
	
	void PrintP() { cout << "x: " << minx << " " << maxx << " y: " << miny << " " << maxy << " z: " << minz << " " << maxz << endl; }
	void PrintD() { cout << "dx: " << Direction.x << " dy: " << Direction.y << " dz: " << Direction.z << endl; }
	void PrintR() { cout << "rx: " << Rotation.x << " ry: " << Rotation.y << " rz: " << Rotation.z << endl; }								 
	
	// ----------  Hit Tests  --------				
	bool HitTest(vector3f& pos, float distance); 
	bool HitTest(vector3f& pos);
	bool HitTest(CModel *pModel);
	bool HitTestMove(CModel *pModel, float offset=0);
	
	
	// Sprite Time
	void ResetTime()						{ SpriteTime = 0; }

    // ---------- Loading Model Geometry ------
    
    virtual bool LoadModel(string filename);
    
	// ----------  Model States -----------
	int GetHealth()							{ return Health; }
	void SetHealth(int health)			    { Health = health; }
	int GetStatus()							{ return Status; }
	void SetStatus(int status)			    { Status = status; }
	
	//  ------- Animation Functions --------------
	virtual int AnimateModel(int startFrame, int endFrame, float percent, bool box=false) { return 0; }
	
    // ---------- Motion Functions ----------------
    
	// Direction = (float)RAD2DEG (atan2(dY, dX)); // tan dir = dX/dY
	
	// returns the direction in form of an angle in the XZ plane
	float GetDirection() { return (float)RAD2DEG (atan2(Direction.z, Direction.x)); }
	
	// set direction for the XZ-plane
	void SetDirection(float y_angle) 
	{  
		// rotation around y-axis
		float x1=float(cos( DEG2RAD (y_angle)));
		float z1=float(-sin( DEG2RAD (y_angle)));
		float y1=0;
		
		SetDirection(x1,y1,z1);
	}

    void SetDirectionRotationTowardsXZ( float x, float z) 
    {
		float y_angle = (float)RAD2DEG (atan2(Position.z-z, x-Position.x));
		SetDirection( y_angle); SetRotation(y_angle);
	} 

	// retrieve direction of the model motion in degrees
	vector3f& GetDirectionV()						{ return Direction; }
	
	void SetDirectionV( vector3f& dir) { SetDirection( dir.x, dir.y, dir.z); }

    void SetDirection( float dX, float dZ) { SetDirection( dX, 0, dZ); }

	// sets the direction
	void SetDirection(float dX, float dY, float dZ)		
	{ 
		float mag=(float)sqrt(dX*dX+dY*dY+dZ*dZ); // normalize direction vector
		if (mag != 0.0)
		{		
		   Direction.x=dX/mag; Direction.y=dY/mag; Direction.z=dZ/mag;
	    }
	    else { Direction.x=Direction.y=Direction.z=0; }    
	}
	
	// get the sprite velocity in units per second
	float GetVelocity()						{ return Velocity; }

	// set model velocity
	void SetVelocity(float vel)			{ Velocity = vel; }

	// set motion
	void SetMotion(float xSpeed, float ySpeed, float zSpeed)	
	{
		Velocity = (float)sqrt(xSpeed*xSpeed+ySpeed*ySpeed+zSpeed*zSpeed);
		
		if (xSpeed != 0.0 || ySpeed != 0.0 || zSpeed != 0.0)
		{		
		   Direction.x=xSpeed/Velocity; Direction.y=ySpeed/Velocity; Direction.z=zSpeed/Velocity;
	    }
	    else { Direction.x=Direction.y=Direction.z=0; }
	}
	void SetVelocity(float xSpeed, float ySpeed, float zSpeed)	{ SetMotion( xSpeed, ySpeed, zSpeed); }

	void SetXVelocity(float vX)				{ SetMotion( vX, GetYVelocity(), GetZVelocity()); }
	void SetYVelocity(float vY)				{ SetMotion( GetXVelocity(), vY, GetZVelocity()); }
	void SetZVelocity(float vZ)				{ SetMotion( GetXVelocity(), GetYVelocity(), vZ); }
	
		
	// retrieve x,y,z component of the velocity vector
	float GetXVelocity()						{ return Velocity*Direction.x; }
	float GetYVelocity()						{ return Velocity*Direction.y; }
	float GetZVelocity()						{ return Velocity*Direction.z; }
	
	// rotation angle of the model
    void Rotate(float rX, float rY, float rZ)		{ Rotation.x += rX;  Rotation.y += rY; Rotation.z += rZ; }
    void SetRotation(float rX, float rY, float rZ)	{ Rotation.x = rX;  Rotation.y = rY; Rotation.z = rZ;}
    vector3f& GetRotationV()							{ return Rotation; }
    
    void Rotate( float rY)						{  Rotation.y += rY; }
    void SetRotation( float y_angle)				{ Rotation.y=y_angle; }
    float GetRotation() 							{ return Rotation.y;}
	
	
	// rotational speed omega in degrees/second
    vector3f& GetOmega()									{ return Omega; }
	void SetOmega(float omgX, float omgY, float omgZ) 	{ Omega.x = omgX; Omega.y = omgY; Omega.z = omgZ; }
	void SetOmega( float omgY) { Omega.y = omgY; }
	void SetOmegaV( vector3f& omg) { Omega.x = omg.x; Omega.y = omg.y; Omega.z = omg.z;}

    // setting a flag
	bool IsMarked()								{ return marked_for_removal; }
    void MarkForRemoval()						{ marked_for_removal = true; }

	
	// Update & Draw
	void SetColor( Uint8 r, Uint8 g, Uint8 b, Uint8 a=100) { Color.Set( (float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)a/100.0f);}
	void SetColor( const CColor& color) 		{ Color.Set( color); }
	void SetAlpha( int alpha) 			{ Color.SetAlpha( (float)alpha/100.0f);}
	void SetFilled( bool fill)			{ filled=fill; }

	void SetTexture( CTexture &texture) { TextureID=texture.GetTextureID(); }
	void SetTextureID( unsigned int texID) { TextureID=texID; }
	         
	// --------   Update and Draw functions ----------------------
	void Update(Uint32 GameTime);
	virtual void Draw(bool box=false);
	
	void SetFrame(int frame) { if (frame < maxFrames) currentFrame=frame; }
	int GetMaxFrames() { return maxFrames;}
	int GetFrame() { return currentFrame;}
	
	
	
	// ------ Child Node functions -------------
	
	void AttachModel(CModel* model) { childNode=model; }
	void DetachModel() { childNode=NULL; }
	void DeleteAttachedModel() { if (childNode!=NULL) delete childNode; childNode=NULL;}
	CModel* GetAttachedModel() { return childNode; }
	
	// clone the current model
	virtual CModel* clone();
	
	
	// clear vertex array data
	virtual void clear();
	
	
	protected:
	
	void DrawBox(bool bbox);
	
	double GetRadius();
	
	void DrawVertex( float x, float y, float z);
	
	void CNodeUpdate();
	
	void SetMaxFrames(int frame) { maxFrames=frame; }
	
	void CalculateNormals();
	
	void DoRotation( vector3f& pos, float yoffset=0.0f);
	
	bool readFace(string& str, vector3f* tmpArrayV, vector3f* tmpArrayN, vector2f* tmpArrayT);
	

};



