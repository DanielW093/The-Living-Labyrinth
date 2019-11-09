#pragma once

#include "Sprite.h"

class CSpritePoint : public CSprite
{
public:
	// Constructor
	CSpritePoint() : CSprite() { Radius = 0.0; }
	
	double Radius;
	
	CSpritePoint(int x, int y, int r=3.0) : CSprite( x, y)
    {
	  Radius = r;
          Color.Set( CColor::Yellow());    
    }
	virtual ~CSpritePoint() {}

	virtual void Draw()
    {
       glColor4f( Color.R, Color.G, Color.B, Color.A);
	   glPointSize( float(2*Radius)); 

		glBegin(GL_POINTS);
	  		glVertex3f( (float)Xpos, (float)Ypos, 0);
		glEnd();
  	}
	
	
};
