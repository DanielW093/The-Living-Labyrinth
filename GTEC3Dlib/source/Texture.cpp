
#include "Texture.h"

	
bool CTexture::LoadTextureFromBitmap( string filename, int frames)
{

  unsigned char* TexData;  // raw texture data
  
  // trying to load bitmap file

  cout << "loading " << filename;
  if ( (TexData=LoadFromBMP( filename.c_str())) == NULL ) 
  {
        cout << " ..failed" << endl;
        cout << "Texture must be a 24-bit or 32-bit bitmap file" << endl;
		return false;
  }
	cout << "..ok" << endl;

	// delete old texture from graphics memory to replace it
	if (TextureID > 0) { glDeleteTextures( 1, &TextureID); TextureID=0; }
 

    // create a new OpenGL texture handle
    glGenTextures( 1, &TextureID );

    // binding texture to handle
    glBindTexture( GL_TEXTURE_2D, TextureID);

    // generate the texture in the GL_RGBA4 format in graphics memory
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA4, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexData );
	
	// generate midmaps		 
	//gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA4, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, TexData );
	
    // linear filtering of texture
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR );
    

	// unbinding current texture
	glBindTexture( GL_TEXTURE_2D, 0);
   
 
    delete TexData; // delete raw texture data
  
    Width=Width/frames;
	
	Frames=frames; frame=1;

  return true;

}

void CTexture::SetZoom( double zoom)
{
	if (zoom > 0)
	{
		
	  Width= int(Width*zoom/Zoom);
	  Height=int(Height*zoom/Zoom);
		
	  Zoom=zoom;
	}	
} 

void CTexture::Draw(int x, int y)
{
   // binding texture
   glBindTexture( GL_TEXTURE_2D, TextureID);  

   glColor4f( 1.0, 1.0, 1.0, 1.0);

   glPushMatrix();
   glTranslatef( (float)x, (float)y, 0);    // transformation to world coordinates
	
	if (Frames > 1)
	{
	  float dx=1.0f/Frames;
	  
	  // todo: select correct texture coordinates
	  glBegin(GL_QUADS);
	    glTexCoord2f((frame-1)*dx, 0.0); glVertex3f(float(-Width/2), float(-Height/2),   0);
	    glTexCoord2f(frame*dx, 0.0); glVertex3f( float(Width/2), float(-Height/2),   0);
	    glTexCoord2f(frame*dx, 1.0); glVertex3f( float(Width/2),  float(Height/2),   0);
	    glTexCoord2f((frame-1)*dx, 1.0); glVertex3f(float(-Width/2),  float(Height/2),   0);
	  glEnd();
		
		
	}
	else
	{
	  glBegin(GL_QUADS);
	    glTexCoord2f(0.0, 0.0); glVertex3f(float(-Width/2), float(-Height/2),   0);
	    glTexCoord2f(1.0, 0.0); glVertex3f( float(Width/2), float(-Height/2),   0);
	    glTexCoord2f(1.0, 1.0); glVertex3f( float(Width/2),  float(Height/2),   0);
	    glTexCoord2f(0.0, 1.0); glVertex3f(float(-Width/2),  float(Height/2),   0);
	  glEnd();
    }
    glPopMatrix();	

  glBindTexture( GL_TEXTURE_2D, 0);
  
}


unsigned char* CTexture::LoadFromBMP(const char* filename)
{

  FILE* stream;
  unsigned char* TexData;
  unsigned char* row;
  unsigned char bft[3];  
  int biSize;               
  int biBitCount;           
  int biCompression;
  int padding = 0;
  int x,y,index=0;
  int overage;

  if ((stream = fopen( filename, "rb")) != NULL)
  {
   // extract file header
   fread( &bft, 1, 2, stream);
   if ( bft[0] != 'B' || bft[1] != 'M' ) { fclose(stream); return NULL; }
   readWord( stream, 12);
   // extract bitmap header
	  biSize = readWord( stream, 4);
    if ( biSize == 12 ) {   // OS/2 style bitmap
    Width = readWord( stream, 2);
    Height = readWord( stream, 2);
    readWord( stream, 2);
    biBitCount = readWord( stream, 2);
	if (biBitCount != 24 ) { fclose(stream); return NULL; }
    }
	else
    {                      // Windows style bitmap
    Width = readWord( stream, 4);
    Height = readWord( stream, 4);
    readWord( stream, 2);
    biBitCount = readWord( stream, 2);
    biCompression = readWord( stream, 4);
    readWord( stream, 12);
    readWord( stream, 4);
    readWord( stream, 4);


	// we don't support compressed or 8 bit images
    if (biBitCount < 24 || biCompression != 0) { fclose(stream); return NULL; }
    }
	
    // ---- extract image data ----

    if (biBitCount == 24)
	{
     overage = (3*Width) % 4;
     if ( overage != 0 ) padding = 4 - overage;

     /* read data row by row and convert to RGBA */
     row = new unsigned char[(3*Width)+padding];

     /* RGBA texture data buffer */
     TexData = new unsigned char[ 4*Width*Height]; 
      
     /* read pixel data in rows
	and convert them into RGBA texture format with colour 0 being the transparent value
     */
    index=0;
	// 24 bit RGB bitmap
    for ( y = 0; y < Height; y++ )
    {
      index=y*4*Width;
      fread(row,1,(3*Width)+padding,stream);
      for ( x = 0; x < 3*Width; x=x+3 ) 
	  {
		  TexData[index++]=row[x+2];
		  TexData[index++]=row[x+1];
		  TexData[index++]=row[x];
		  if (row[x] == R && row[x+1] == G && row[x+2]==B) TexData[index]=0; // is transparent
		  else TexData[index]=255;
		  index++;
	  }
    }
   }
   else  // 32 bit RGBA bitmap
   {
     /* read data row by row and convert to RGBA */
     row = new unsigned char[(4*Width)];

     /* RGBA texture data buffer */
     TexData = new unsigned char[ 4*Width*Height]; 

	 index=0;
     for ( y = 0; y < Height; y++ )
     {
      index=y*4*Width;
      fread(row,1,4*Width,stream);
      for ( x = 0; x < 4*Width; x=x+4 ) 
	  {
		  TexData[index++]=row[x+2];
		  TexData[index++]=row[x+1];
		  TexData[index++]=row[x];
		  TexData[index++]=row[x+3];
	  }

	 }
   }
	   
    fclose( stream);
    delete row; return TexData;
    }
    return NULL;
}


int CTexture::readWord(FILE* in, int len)
{
   unsigned char* v;
   int x,temp = 0;


   v = new unsigned char[len*sizeof(int)];
   fread( v, len, 1, in);
   for ( x = 0; x < len; x++ )
   {
     temp += ( ((int)v[x]) << (x * 8));
   }
   delete v;
   return temp;
}	






