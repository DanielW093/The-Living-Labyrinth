#pragma once

#include "Sprite.h"

class CSpriteTriangle : public CSprite
{
public:

	// Constructors & Destructor
    CSpriteTriangle() : CSprite() {  Rotation=90;}
	
	CSpriteTriangle(int x, int y, int w, int h, bool fill=true) : CSprite( x, y)
    {
        Width=w; Height=h; Rotation=90; filled=fill;
    }

    CSpriteTriangle(int x, int y, int width, int height, const CColor& color, bool fill=true) : CSprite( x, y)
    {
        Width=width; Height=height; Rotation=90; Color.Set( color); filled=fill;
    }


	virtual ~CSpriteTriangle() {}

	virtual void Draw()
    {
         if (!filled) glPolygonMode( GL_FRONT, GL_LINE);       	

    	glColor4f( Color.R, Color.G, Color.B, Color.A);
	
    	glPushMatrix();
		  glTranslatef( (float)Xpos, (float)Ypos, 0);  // translation
		  glRotatef( float(Rotation-90), 0, 0, 1 );  // rotation

		  glBegin(GL_TRIANGLES);
		   glVertex3f( float(-Width/2), float(-Height/2),   0);
		   glVertex3f( float(Width/2), float(-Height/2),   0);
		   glVertex3f(  0,  float(Height/2),   0);
		  glEnd();
		glPopMatrix();
	
	if (!filled) glPolygonMode( GL_FRONT, GL_FILL);

  	}

	// true if the position (x,y) is inside the bounding box of this sprite
	virtual bool HitTest(int x, int y)
	{
       double x1=-Width/2;
	   double x2=Width/2;
	   double y1=0;
	   double y2=-Height/2;

	   double xp,yp;
	   
	   // correcting for bounding box rotation
           xp = (x-Xpos)*cos(DEG2RAD (-Rotation)) - (y-Ypos)*sin(DEG2RAD (-Rotation));
	   yp = (x-Xpos)*sin(DEG2RAD (-Rotation)) + (y-Ypos)*cos(DEG2RAD (-Rotation));

	   // checking collsion with bottom rectangle
       if (xp >= x1 && xp <= x2 && yp >= y1 && yp <= y2) return true;


	   // top smaller bounding rectangle
	   x1=-Width/4; x2=Width/4; y1=Height/2; y2=0;
	   // checking collsion with top smaller rectangle
       if (xp >= x1 && xp <= x2 && yp >= y1 && yp <= y2) return true;



		
	   return false;
	}
	
	
};

