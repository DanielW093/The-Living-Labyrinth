
#include "Floor.h"

void CFloor::Draw(int x, int z, bool grid)
{
  float texXR=1.0f, texYR=1.0f;
	
  if (grid)
  {
  glColor3f( 0, 1.0f, 0);  // green is the grid
  glLineWidth(2.0f);
	
  glPushMatrix();
    glTranslatef( (float)x, 0, (float)z);  	// transformation to world coordinates	
	
	 glBegin(GL_LINES);
	
     for (int n=-Width/2; n <= Width/2; n=n+50)
     {
	   glVertex3i( n, 0, -Depth/2); 
	   glVertex3i( n, 0, Depth/2); 
	 }
	 
	 for (int n=-Depth/2; n <= Depth/2; n=n+50)
     {
	   glVertex3i( -Width/2, 0, n); 
	   glVertex3i( Width/2, 0, n); 
	 }
	glEnd();
   }

    if (TextureID)
    {
	 glColor4f(1.0,1.0,1.0, 1.0);
	 
	 if (tiling)
	 {
	  texXR=float(Width)/texW;
	  texYR=float(Depth)/texH;
     }
	 
	 glBindTexture( GL_TEXTURE_2D, TextureID);  // binding texture
     glBegin(GL_QUADS);
      glNormal3f(0,1,0);
	  glTexCoord2f(0.0, texYR); glVertex3i(-Width/2, -1, -Depth/2);
	  glTexCoord2f(texXR, texYR); glVertex3i( Width/2, -1, -Depth/2);
	  glTexCoord2f(texXR, 0.0); glVertex3i( Width/2, -1,  Depth/2);
	  glTexCoord2f(0.0, 0.0); glVertex3i(-Width/2, -1,  Depth/2);
     glEnd();
	 glBindTexture( GL_TEXTURE_2D, 0);
	}
	else
	{
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	
     glColor3f(color.R, color.G, color.B);
     glBegin(GL_QUADS);
	  glVertex3i(-Width/2, -1, -Depth/2);
	  glVertex3i( Width/2, -1, -Depth/2);
	  glVertex3i( Width/2, -1,  Depth/2);
	  glVertex3i(-Width/2, -1,  Depth/2);
     glEnd();
     
    }
     
     
  glPopMatrix();


}


