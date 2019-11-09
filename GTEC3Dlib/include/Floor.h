#pragma once

#include "GTEC.h"
#include "Model.h"

using namespace std;

class CFloor
{
public:
	
	// ----- Constructors & Destructor -----
	
	CFloor() { Width=Depth=Xpos=Zpos=texW=texH=0; TextureID=0; tiling=false; color.Set(0, 0.33f, 0, 1.0); }
	
	~CFloor() { glDeleteTextures( 1, &TextureID); }

	// ----- Attributes ------

private:
	
	// Position & Size
	int Width, Depth, Xpos, Zpos; // width and depth of texture in pixels
    GLuint TextureID;			   // OpenGL texture ID
	CColor color;				   // colour
	bool tiling;				   // should the texture be tiled?
	int texW, texH;		   			// texture width/height

	
public:
	

	// Size of the Texture
	int GetWidth()			    { return Width; }
	int GetDepth()		            { return Depth; }
    int GetHeight( int x, int z)	    { return 0; }
	void SetSize(int x, int z)         { Width=x; Depth=z; }
	void SetPosition( int x, int z)    { Xpos=x; Zpos=z; }
	
	void SetTexture( CTexture &texture)	{ TextureID=texture.GetTextureID(); texW=texture.GetWidth(); texH=texture.GetHeight();}
	void SetColor( const CColor& c) { color.Set( c);  }
	
	void ClearTexture() { TextureID=0; }
	void SetTiling( bool tile) { tiling=tile;}
    
    void Draw(bool grid=false)					{ Draw( Xpos, Zpos, grid);}
	void Draw(int x, int z, bool grid=false);

private:

        

	
};



