#pragma once

#include "GTEC.h"
#include "Color.h"
#include "Texture.h"


using namespace std;

class CSprite
{
public:
	
	// ----- Constructors & Destructor -----
	
	CSprite();
	CSprite(int x, int y);
	CSprite(int x, int y, int z);
	CSprite( int x, int y, CTexture &texture);

	virtual ~CSprite() {}

	// ----- Attributes ------

protected:

    // Appearance
	CColor Color;			// sprite colour
	GLuint TextureID;  		// texture ID, zero if no texture assigned
	bool filled;
	bool looping;		 
	
	// Animation variables
	int Frames;	    		// number of frames
	int startFrame, stopFrame;
	double frame, period;
	
	// Position & Size
	double Xpos, Ypos,Zpos;		// position is the center point of the sprite 
	int Width, Height;		// width and height of sprite
	double Zoom;

	// Sprite Time
	Uint32 SpriteTime;		// reference time
	
	// Sprite states
	int Status;				// user defined status
	int Health;				// health
	bool marked_for_removal;
	
	
	// Dynamics - Linear
	double Velocity;		// movement velocity in pixel/second
	double Direction;      // movement direction in degrees
	
	// Dynamics - Rotational
	double Rotation;		// rotation (orientation) in degrees (zero pointing up)
	double Omega;			// rotational velocity in degrees per second


public:
	
	// Position functions
	int GetX()								{ return (int)Xpos; }
	int GetY()								{ return (int)Ypos; }
	int GetZ()								{ return (int)Zpos;}

	void SetZ( int z)						{Zpos = (double)z;}
	void SetX( int x)                      { Xpos = (double)x; }
	void SetY( int y)                      { Ypos = (double)y; }

	void SetPosition(double x, double y)	{ Xpos = x; Ypos = y;}
	void Move(double dx, double dy)			{ Xpos += dx; Ypos += dy; }

	// Size functions
	int GetWidth()							{ return Width; }
	int GetHeight()							{ return Height; }
	void SetSize(int w, int h);
	void SetZoom(double zoom);
	double GetZoom()						{ return Zoom; }
	void GetLocalPosition( int &x, int &y);
	double GetRadius();							 
	
	// -------  Hit Tests-------				
	bool HitTest(int x, int y, double radius);
	virtual bool HitTest(int x, int y);
	virtual bool HitTest(int x,int y,int z);
	virtual bool HitTest(CSprite *pSprite);
	
	
	// Sprite Time
	void ResetTime()						{ SpriteTime = 0; }

	
	// Sprite State
	int GetHealth()							{ return Health; }
	void SetHealth(int health)			    { Health = health; }
	int GetStatus()							{ return Status; }
	void SetStatus(int status)			    { Status = status; }
	
	
	//  ------- Animation Functions --------------
	void SetFrame( int f)					{ frame=(double)f; period=0.0; startFrame=f; stopFrame=f;}
	void NextFrame()					    { frame++; if (frame > Frames) SetFrame( 1); }
	int GetFrame()							{ return (int)frame; }	
	
	void PlayAnimation( int start, int stop, double speed, bool loop=false);
	
	bool AnimationFinished()				{ return stopFrame==frame;}
	
	
    // ---------- Motion Functions ----------------
    
	
	// retrieve direction of the sprite motion in degrees
	double GetDirection()						{ return Direction; }
	
	// sets the direction
	void SetDirection(double dir)				{ Direction = dir; } 
	void SetDirection(double dX, double dY)		
	{ 
		Direction = (double)RAD2DEG (atan2(dY, dX)); // tan dir = dX/dY
	}
	
	// get the sprite velocity in pixels per second
	double GetVelocity()						{ return Velocity; }

	// set sprite velocity
	void SetVelocity(double vel)				{ Velocity = vel; }

	// set motion from horizontal and vertical speed.
	void SetMotion(double hSpeed, double vSpeed)	
	{
		SetVelocity( (double)sqrt( (hSpeed*hSpeed)+(vSpeed*vSpeed)));
		SetDirection( hSpeed, vSpeed);
	}
	void SetVelocity(double hSpeed, double vSpeed)	{ SetMotion( hSpeed, vSpeed); }

	void SetXVelocity(double vH)				{ SetMotion( vH, GetYVelocity()); }
	void SetYVelocity(double vV)				{ SetMotion( GetXVelocity(), vV); }
		
	// retrieve horizontal & vertical component of the velocity vector
	double GetXVelocity()						{ return Velocity*cos( DEG2RAD (Direction)); }
	double GetYVelocity()						{ return Velocity*sin( DEG2RAD (Direction)); }
	
	// rotation angle of the sprite
    void Rotate(double rot)					{ Rotation += rot; if (Rotation >= 360) Rotation -= 360;}
    void RotateY(double rot);
	void SetRotation(double dX, double dY)	{ Rotation = RAD2DEG( atan2(dY, dX)); }
    void SetRotation( double rot)		{ Rotation=rot; }
	double GetRotation()						{ return Rotation; }

	// rotation speed omega in degrees/second
    double GetOmega()							{ return Omega; }
	void SetOmega(double omg) 					{ Omega = omg; }

	bool IsMarked()								{ return marked_for_removal; }
    void MarkForRemoval()						{ marked_for_removal = true; }

	
	// Update & Draw
	void SetColor( Uint8 r, Uint8 g, Uint8 b, Uint8 a=100) { Color.Set( (float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)a/100.0f);}
	void SetColor( const CColor& color) 		{ Color.Set( color); }
	void SetAlpha( int alpha) 			{ Color.SetAlpha( (float)alpha/100.0f);}
	void SetFilled( bool fill)			{ filled=fill; }

	void SetTexture( CTexture &texture);  
	         
	// Update and Draw function
	void Update(Uint32 GameTime);
	virtual void Draw();

};



