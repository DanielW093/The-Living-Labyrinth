#pragma once

#include "Sprite.h"

class CSpriteRectangle : public CSprite
{
public:
	
	// Constructors & Destructor
	CSpriteRectangle() : CSprite() {}
	
	
	CSpriteRectangle(int x, int y, int width, int height, bool fill=true) : CSprite( x, y)
    {
        Width=width; Height=height; Color.Set( CColor::Gray()); filled=fill;
    }


	CSpriteRectangle(int x, int y, int width, int height, const CColor& color, bool fill=true) : CSprite( x, y)
    {
        Width=width; Height=height; Color.Set( color); filled=fill;
    }
	
	virtual ~CSpriteRectangle() {}




    // draw the rectangle as an OpenGL Quad 
	virtual void Draw()
    {
		
        if (!filled) glPolygonMode( GL_FRONT, GL_LINE);
		
    	glColor4f( Color.R, Color.G, Color.B, Color.A);
	
    	glPushMatrix();
		  glTranslatef( (float)Xpos, (float)Ypos, 0);  	// transformation to world coordinates
		  glRotatef( (float)Rotation, 0, 0, 1 );   // rotation in degrees

		  glBegin(GL_QUADS);
		   glVertex3f(float(-Width/2), float(-Height/2),   0);
		   glVertex3f(float(Width/2), float(-Height/2),   0);
		   glVertex3f(float(Width/2),  float(Height/2),   0);
		   glVertex3f(float(-Width/2),  float(Height/2),   0);
		  glEnd();
		glPopMatrix();

		if (!filled) glPolygonMode( GL_FRONT, GL_FILL);
	}
	
};
