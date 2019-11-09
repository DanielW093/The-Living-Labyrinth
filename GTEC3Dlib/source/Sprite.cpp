

#include "Sprite.h"


CSprite::CSprite()
{
	Xpos=Zpos=Ypos=0.0;		
	Width=Height=0;
	Zoom=100.0;

	TextureID=0; 
	
	Frames=1; frame=1.0; period=0.0; looping=false;
	startFrame=stopFrame=1;

	Color.Set( 1.0, 0.0, 0.0, 1.0); // red
	filled=true;
	
	
	// Time
	SpriteTime=0;
	
	Health=100;	Status=0;		
	
	// Linear Motion
	Velocity=0.0;		
	Direction=0.0;    
	
	// Rotation
	Rotation=0.0;		
	Omega=0.0;

	marked_for_removal=false;  
}

CSprite::CSprite(int x, int y,int z)
{
	Xpos = (double)x; Ypos = (double)y;	Zpos = (double)z;
	Width=Height=0;	
    Zoom=100.0;

	TextureID=0; 
	Frames=1; frame=1.0; period=0.0; looping=false;
	startFrame=stopFrame=1;

	Color.Set( 1.0, 0.0, 0.0, 1.0); // red
	filled=true;
	
	// Time
	SpriteTime=0;
	
	Health=100;	Status=0;		
	
	// Linear Motion
	Velocity=0.0;		
	Direction=0.0;         
	
	// Rotation
	Rotation=0.0;		
	Omega=0.0;	

	marked_for_removal=false;
}

CSprite::CSprite(int x, int y)
{
	Xpos = (double)x; Ypos = (double)y ;Zpos = 0.0;
	Width=Height=0;	
    Zoom=100.0;

	TextureID=0; 
	Frames=1; frame=1.0; period=0.0; looping=false;
	startFrame=stopFrame=1;

	Color.Set( 1.0, 0.0, 0.0, 1.0); // red
	filled=true;
	
	// Time
	SpriteTime=0;
	
	Health=100;	Status=0;		
	
	// Linear Motion
	Velocity=0.0;		
	Direction=0.0;         
	
	// Rotation
	Rotation=0.0;		
	Omega=0.0;	

	marked_for_removal=false;
}



CSprite::CSprite( int x, int y, CTexture &texture)
{

	Xpos = (double)x; Ypos = (double)y;
	Zoom=100.0;

	Width = texture.GetWidth();
	Height = texture.GetHeight();
   
	TextureID = texture.GetTextureID();
	filled=true;
	
	Frames = texture.GetFrames();
	frame=1; period=0; looping=false;
	startFrame=stopFrame=1;

	Color.Set( 1.0, 1.0, 1.0, 1.0); // White
	
	// Time
	SpriteTime=0;
	
	Health=100;	Status=0;		
	
	// Linear Motion
	Velocity=0.0;		
	Direction=0.0;       
	
	// Rotation
	Rotation=0.0;		
	Omega=0.0;	

	marked_for_removal=false;
}


void CSprite::SetSize(int w, int h)
{
	Width = w; Height = h;
}

void CSprite::SetZoom( double zoom)
{
	if (zoom > 0)
	{
		
	  Width= int(Width*zoom/Zoom);
	  Height=int(Height*zoom/Zoom);
		
	  Zoom=zoom;
	}	
} 

void CSprite::SetTexture( CTexture &texture)
{

    Width = texture.GetWidth();
	Height = texture.GetHeight();
	
	Zoom = texture.GetZoom();
   
	TextureID = texture.GetTextureID();
	
	Frames = texture.GetFrames();
	frame=1;

	Color.Set( 1.0, 1.0, 1.0, 1.0);
}


double CSprite::GetRadius()
{
	
   return sqrt( (Width/2.0)*(Width/2.0)+(Height/2.0)*(Height/2.0));

}

void CSprite::GetLocalPosition( int &x, int &y)
{
  int xp,yp;
	
  xp = (int)((x)*cos(DEG2RAD (-Rotation)) - (y)*sin(DEG2RAD (-Rotation)));
  yp = (int)((x)*sin(DEG2RAD (-Rotation)) + (y)*cos(DEG2RAD (-Rotation)));

  x=xp; y=yp;

}

// ----------  Collision detection -----------


// true if the distance between this sprite and the position (x,y) is less than the specified minDistance 
bool CSprite::HitTest(int x, int y, double minDistance)
{
	double distance=sqrt((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos));
	
	if (distance < minDistance) return true;
	return false;
}


// true if the position (x,y) is inside the bounding box of this sprite
bool CSprite::HitTest(int x, int y)
{
    double x1=-Width/2;
    double x2=Width/2;
	double y1=-Height/2;
	double y2=Height/2;

	double xp,yp,zp;
	   
	// correcting for bounding box rotation
    xp = (x-Xpos)*cos(DEG2RAD (-Rotation)) - (y-Ypos)*sin(DEG2RAD (-Rotation));
	yp = (x-Xpos)*sin(DEG2RAD (-Rotation)) + (y-Ypos)*cos(DEG2RAD (-Rotation));

    if (xp >= x1 && xp <= x2 && yp >= y1 && yp <= y2) return true;
	return false;
}

bool CSprite::HitTest(int x,int y,int z)
{
	double z1 = -Width/2;
	double z2 = Width/2;
	double x1=-Width/2;
    double x2=Width/2;
	double y1=-Height/2;
	double y2=Height/2;

	double xp,yp,zp;
	   
  // bounding box rotation around the y-axis
     xp = x*cos(DEG2RAD (-Rotation)) + z*sin(DEG2RAD (-Rotation));
	 zp = -x*sin(DEG2RAD (-Rotation)) + z*cos(DEG2RAD (-Rotation));
	 yp = y;
	 
	 x=xp; y=yp;  z=zp;

    if (xp >= x1 && xp <= x2 && yp >= y1 && yp <= y2 && zp >= z1 && zp <= z2) return true;
	return false;
}

// are bounding rectangles overlapping?
	
	bool CSprite::HitTest(CSprite *pSprite) 
    { 
       double Xpos2=pSprite->GetX();
	   double Ypos2=pSprite->GetY();
		
       double distance=sqrt((Xpos2-Xpos)*(Xpos2-Xpos)+(Ypos2-Ypos)*(Ypos2-Ypos));
	   double radius1=GetRadius(); 
	   double radius2=pSprite->GetRadius();

       // sprites are too far apart and could not possibly overlap
	   if (distance > radius1+radius2) return false;

	   // centre of pSprite inside this sprite?
       if ( HitTest( (int)Xpos2, (int)Ypos2)) return true;
       
       // centre of pSprite inside this sprite?
       if ( pSprite->HitTest( (int)Xpos, (int)Ypos)) return true;
       
     
       // ----- 4 bounding points of this sprite inside pSprite? ----

       // top left hand corner    
       Xpos2 = Xpos + (-Width/2)*cos(DEG2RAD (Rotation)) - (Height/2)*sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + (-Width/2)*sin(DEG2RAD (Rotation)) + (Height/2)*cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( (int)Xpos2, (int)Ypos2)) return true;
       // top right hand corner 
       Xpos2 = Xpos + (Width/2)*cos(DEG2RAD (Rotation)) - (Height/2)*sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + (Width/2)*sin(DEG2RAD (Rotation)) + (Height/2)*cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( (int)Xpos2, (int)Ypos2)) return true;
       // bottom right hand corner
       Xpos2 = Xpos + (Width/2)*cos(DEG2RAD (Rotation)) - (-Height/2)*sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + (Width/2)*sin(DEG2RAD (Rotation)) + (-Height/2)*cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( (int)Xpos2, (int)Ypos2)) return true;
       // bottom left hand corner   
       Xpos2 = Xpos + (-Width/2)*cos(DEG2RAD (Rotation)) - (-Height/2)*sin(DEG2RAD (Rotation));
	   Ypos2 = Ypos + (-Width/2)*sin(DEG2RAD (Rotation)) + (-Height/2)*cos(DEG2RAD (Rotation));
       if (pSprite->HitTest( (int)Xpos2, (int)Ypos2)) return true;

       
       // ----- 4 bounding points of pSprite inside this sprite? ----
         
       // top left hand corner    
       Xpos2 = pSprite->Xpos + (-pSprite->Width/2)*cos(DEG2RAD (pSprite->Rotation)) - (pSprite->Height/2)*sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + (-pSprite->Width/2)*sin(DEG2RAD (pSprite->Rotation)) + (pSprite->Height/2)*cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( (int)Xpos2, (int)Ypos2)) return true;
       // top right hand corner 
       Xpos2 = pSprite->Xpos + (pSprite->Width/2)*cos(DEG2RAD (pSprite->Rotation)) - (pSprite->Height/2)*sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + (pSprite->Width/2)*sin(DEG2RAD (pSprite->Rotation)) + (pSprite->Height/2)*cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( (int)Xpos2, (int)Ypos2)) return true;
       // bottom right hand corner
       Xpos2 = pSprite->Xpos + (pSprite->Width/2)*cos(DEG2RAD (pSprite->Rotation)) - (-pSprite->Height/2)*sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + (pSprite->Width/2)*sin(DEG2RAD (pSprite->Rotation)) + (-pSprite->Height/2)*cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( (int)Xpos2, (int)Ypos2)) return true;
       // bottom left hand corner   
       Xpos2 = pSprite->Xpos + (-pSprite->Width/2)*cos(DEG2RAD (pSprite->Rotation)) - (-pSprite->Height/2)*sin(DEG2RAD (pSprite->Rotation));
	   Ypos2 = pSprite->Ypos + (-pSprite->Width/2)*sin(DEG2RAD (pSprite->Rotation)) + (-pSprite->Height/2)*cos(DEG2RAD (pSprite->Rotation));
       if (HitTest( (int)Xpos2, (int)Ypos2)) return true;
      
      
       return false;
    }





// -----------    Update -----------------

void CSprite::Update(Uint32 GameTime)
{
	if (SpriteTime == 0) { SpriteTime = GameTime; return; }

	int deltaTime = GameTime - SpriteTime;
	if ( deltaTime < 0) deltaTime = 0;

	// linear motion update
    double dx = Velocity*cos( DEG2RAD (Direction))*deltaTime/1000.0;
	double dy = Velocity*sin( DEG2RAD (Direction))*deltaTime/1000.0;

    Xpos += dx; Ypos += dy;
	
	// rotation update
	Rotate( Omega*deltaTime/1000.0);
	
	// animation update
	if (period > 0)
	{
	  frame=frame+deltaTime*period/1000.0;
	  if (frame >= stopFrame+1) 
	  { 
		if (looping) frame=startFrame;
		else SetFrame( stopFrame); 
	  }
    }
    
    SpriteTime = GameTime;
}


void CSprite::PlayAnimation( int start, int stop, double speed, bool loop)
{
	  if (frame <= start || frame >= stop+1 || period == 0)
	  {
	    startFrame=start; stopFrame=stop; period=speed;
	    frame = start; looping=loop;
	  }
	
}

void CSprite::RotateY(double rot)
{
	glRotatef( (float)Rotation, 0, 1,0 );  // rotation
}

void CSprite::Draw()  
{
    glColor4f( Color.R, Color.G, Color.B, Color.A);
	
    if (TextureID  <  1)
	{
    // ----------  If no texture assigned draw a green cross instead --------
   
     glPushMatrix();
		glTranslatef( (float)Xpos, (float)Ypos, (float)Zpos);  // transformation to world coordinates
		glRotatef( (float)Rotation, 0, 0, 1 );  // rotation
	
		glBegin(GL_LINES);
	  	  glVertex3f( -5.0, 5.0, 0);
	      glVertex3f( 5.0, -5.0, 0);
	      glVertex3f( -5.0, -5.0, 0);
	      glVertex3f( 5.0, 5.0, 0);
		glEnd();
	 glPopMatrix();
	}
	else  // draw texture as a rectangular sprite
	{

		// binding texture
   		glBindTexture( GL_TEXTURE_2D, TextureID);  

   		glPushMatrix();
   		glTranslatef( (float)Xpos, (float)Ypos, 0);    // transformation to world coordinates
		glRotatef( (float)Rotation, 0, 0, 1 );   // rotation in degrees
	
	   if (Frames > 1)  // animated texture: select the correct frame
	   {
		double dx=1.0/Frames;
		glBegin(GL_QUADS);
			glTexCoord2f(float(((int)frame-1)*dx), 0.0); glVertex3f(float(-Width/2), float(-Height/2),   0);
			glTexCoord2f(float((int)frame*dx), 0.0); glVertex3f( float(Width/2), float(-Height/2),   0);
			glTexCoord2f(float((int)frame*dx), 1.0); glVertex3f( float(Width/2),  float(Height/2),   0);
			glTexCoord2f(float(((int)frame-1)*dx), 1.0); glVertex3f(float(-Width/2),  float(Height/2),   0);
		glEnd();	
	   }
	   else  // no anmimated texture
	   {
		glBegin(GL_QUADS);
	  		glTexCoord2f(0.0f, 0.0f); glVertex3f(float(-Width/2), float(-Height/2),   0);
	  		glTexCoord2f(1.0f, 0.0f); glVertex3f( float(Width/2), float(-Height/2),   0);
	  		glTexCoord2f(1.0f, 1.0f); glVertex3f( float(Width/2),  float(Height/2),   0);
	  		glTexCoord2f(0.0f, 1.0f); glVertex3f(float(-Width/2),  float(Height/2),   0);
		glEnd();
		
	   }
    	glPopMatrix();	

  		glBindTexture( GL_TEXTURE_2D, 0);

		
	}
}
