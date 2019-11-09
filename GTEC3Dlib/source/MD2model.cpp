#include "MD2model.h"

#include <iostream>
using namespace std;



// ModelMd2 constructor
CModelMd2::CModelMd2() : CModel()
{
     numVertices = 0;    // vertices
     numTriangles = 0;   // triangles
     numFrames = 0;      // frames
     numST = 0;          // texture coordinates
     frameSize = 0;      // needed?
     currentFrame = 0;   // current keyframe 
     nextFrame = 1;      // next keyframe
     interpol = 0.0;     // interpolation percent
     triIndex = NULL;    // triangle indices
     st = NULL;          // texture coordinate indices
     vertexList = NULL;  // vertices
     maxFrames=0;
	

	minx = 100000;
	maxx= -100000;
	miny = 100000;
	maxy= -100000;
	minz = 100000;
	maxz= -100000;
	bbsize = 0.0;
	
}    

// CModelMd2 destructor
CModelMd2::~CModelMd2()
{
	clear();
}


// CMD2Model::LoadModel()
// access: public
// desc: loads model from file
bool CModelMd2::LoadModel(string modelFile)
{
     FILE *filePtr;                          // file pointer
     int fileLen;                            // length of model file
     char *buffer;                           // file buffer
     
     modelHeader_t *modelHeader;             // model header
     
     stIndex_t *stPtr;                       // texture data
     frame_t *frame;                         // frame data
     vector_t *vertexListPtr;                // index variable
     mesh_t *bufIndexPtr;         // index variables
     int i, j;                               // index variables
     
     cout << "loading... " << modelFile; 
     // open the model file
     
     filePtr = fopen(modelFile.c_str(), "rb");
     if (filePtr == NULL)
     {
		 cout << "fail" << endl;
         return false;
          
	  }
     
     // find length of file
     fseek(filePtr, 0, SEEK_END);
     fileLen = ftell(filePtr);
     fseek(filePtr, 0, SEEK_SET);
     
     cout << "..ok";
     
     // read entire file into buffer
     buffer = new char [fileLen+1];
     fread(buffer, sizeof(char), fileLen, filePtr);
     
     // extract model file header from buffer
     modelHeader = (modelHeader_t*)buffer;
     
     // allocate vertex list
     vertexList = new vector_t [modelHeader->numXYZ * modelHeader->numFrames];
     
     numVertices = modelHeader->numXYZ;
     numFrames = modelHeader->numFrames;
     frameSize = modelHeader->framesize;
     
     //cout << "Number of vertices: " << numVertices << endl;
     //cout << "Number of frames: " << numFrames << endl;
     //cout << "Framesize: " << frameSize << endl;
     
     maxFrames=numFrames;
     
     for (j = 0; j < numFrames; j++)
     {
          frame = (frame_t*)&buffer[modelHeader->offsetFrames + frameSize * j];
          
          //cout << frame->name << endl;
          
          vertexListPtr = (vector_t*)&vertexList[numVertices * j];
          for (i = 0; i < numVertices; i++)
          {
               vertexListPtr[i].point[0] = frame->scale[0] * frame->fp[i].v[0] + frame->translate[0];
               vertexListPtr[i].point[2] = frame->scale[1] * frame->fp[i].v[1] + frame->translate[1];
               vertexListPtr[i].point[1] = frame->scale[2] * frame->fp[i].v[2] + frame->translate[2];
               
               if (j==0)  // calculate size from first frame only
               {
				   minx = (vertexListPtr[i].point[0]<minx)?vertexListPtr[i].point[0]:minx;
				   miny = ( vertexListPtr[i].point[1]<miny)? vertexListPtr[i].point[1]:miny;
				   minz = (vertexListPtr[i].point[1]<minz)?vertexListPtr[i].point[1]:minz;
		           maxx = (vertexListPtr[i].point[0]>maxx)?vertexListPtr[i].point[0]:maxx;
		           maxy = ( vertexListPtr[i].point[1]>maxy)? vertexListPtr[i].point[1]:maxy;
		           maxz = (vertexListPtr[i].point[1]>maxz)?vertexListPtr[i].point[1]:maxz;
			   }
               
               
               //cout << i << " " << vertexListPtr[i].point[0] << "," << vertexListPtr[i].point[1] << "," << vertexListPtr[i].point[2] << endl;
          }
     }
     
     numST = modelHeader->numST;
     
     // create texture coordinates array
     st = new texCoord_t [numST];
     
     stPtr = (stIndex_t*)&buffer[modelHeader->offsetST];
     
     for (i = 0; i < numST; i++)
     {
          st[i].s = (float)stPtr[i].s;
          st[i].t = (float)stPtr[i].t;
		  //cout << i << " " << stPtr[i].s << " " << stPtr[i].t << endl;
     }
     
     numTriangles = modelHeader->numTris;
     triIndex = new mesh_t [numTriangles];
     
     //cout << "Number of Triangles: " << numTriangles << endl;
     
     // point to triangle indexes in buffer
     bufIndexPtr = (mesh_t*)&buffer[modelHeader->offsetTris];
     
     
     // create a mesh (triangle) list
     // for all triangles in each frame
          for(i = 0; i < numTriangles; i++)
          {
               triIndex[i].meshIndex[0] = bufIndexPtr[i].meshIndex[0];
               triIndex[i].meshIndex[1] = bufIndexPtr[i].meshIndex[1];
               triIndex[i].meshIndex[2] = bufIndexPtr[i].meshIndex[2];
               triIndex[i].stIndex[0] = bufIndexPtr[i].stIndex[0];
               triIndex[i].stIndex[1] = bufIndexPtr[i].stIndex[1];
               triIndex[i].stIndex[2] = bufIndexPtr[i].stIndex[2];   
          }
     
     // close file and free memory
     fclose(filePtr);
	delete buffer;
     
    
     currentFrame = 0;
     nextFrame = 1;
     interpol = 0.0;
     
     cout << " Triang:" << numTriangles << " Frames:" << numFrames << " X:" << int(maxx+minx) << " Y:" << int(maxy-miny) << " Z:" << int(maxz-minz)  << endl;
     
     return 0;
}



void CModelMd2::SetTexture(CTexture &texture)
{
     int i;
     
     TextureID=texture.GetTextureID();
     
     cout << "Texture: " << texture.GetWidth() << " x " << texture.GetHeight() << endl;
     
     for (i = 0; i < numST; i++)
     {
          st[i].s = st[i].s/(float)texture.GetWidth();
          st[i].t = 1.0f-st[i].t/(float)texture.GetHeight(); // texture is upside down
                   
     }
}


int CModelMd2::AnimateModel(int startFrame, int endFrame, float percent, bool box)
{
     vector_t *vList;              // current frame vertices
     vector_t *nextVList;          // next frame vertices
     int i;                       // index counter
     float x1, y1, z1;            // current frame point values
     float x2, y2, z2;            // next frame point values
     
     vector_t vertex[3]; 
     
     if ((startFrame > currentFrame))
          currentFrame = startFrame;
     
     if ((startFrame < 0) || (endFrame < 0))
          return -1;
     
     if ((startFrame >= numFrames) || (endFrame >= numFrames))
          return -1;
     
     if (interpol >= 1.0)
     {
          interpol = 0.0f;
          currentFrame++;
          if (currentFrame >= endFrame)
               currentFrame = startFrame;
          
          nextFrame = currentFrame + 1;
          
          if (nextFrame >= endFrame)
               nextFrame = startFrame;
         
     }
 
     vList = &vertexList[numVertices*currentFrame];
     nextVList = &vertexList[numVertices*nextFrame];
 

	glColor4f(1.0, 1.0, 1.0, 1.0);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    
    if (TextureID==0) 
    {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0);	
	}
    
    glPushMatrix();
	 
        glTranslatef(Position.x, Position.y, Position.z);
        glRotatef( Rotation.x, 1, 0, 0 );   // rotation around x-axis
	    glRotatef( Rotation.y, 0, 1, 0 );   // rotation around y-axis
	    glRotatef( Rotation.z, 0, 0, 1 );   // rotation around z-axis
	    glScalef( Scale, Scale, Scale);
    
		minx = 100000;
		maxx= -100000;
		miny = 100000;
		maxy= -100000;
		minz = 100000;
		maxz= -100000;
		bbsize = 0.0;
     
    glBegin(GL_TRIANGLES);
     for (i = 0; i < numTriangles; i++)
     {
          // get first points of each frame
          x1 = vList[triIndex[i].meshIndex[0]].point[0];
          y1 = vList[triIndex[i].meshIndex[0]].point[1];
          z1 = vList[triIndex[i].meshIndex[0]].point[2];

		  minx = (x1<minx)?x1:minx;
		  miny = (y1<miny)?y1:miny;
		  minz = (z1<minz)?z1:minz;
		  maxx = (x1>maxx)?x1:maxx;
		  maxy = (y1>maxy)?y1:maxy;
		  maxz = (z1>maxz)?z1:maxz;
		  if (maxx-minx>bbsize)
			  bbsize = (maxx-minx);
		  if (maxy-miny>bbsize)
			  bbsize = (maxy-miny);
		  if (maxz-minz>bbsize)
			  bbsize = (maxz-minz);

          x2 = nextVList[triIndex[i].meshIndex[0]].point[0];
          y2 = nextVList[triIndex[i].meshIndex[0]].point[1];
          z2 = nextVList[triIndex[i].meshIndex[0]].point[2];

		  minx = (x2<minx)?x2:minx;
		  miny = (y2<miny)?y2:miny;
		  minz = (z2<minz)?z2:minz;
		  maxx = (x2>maxx)?x2:maxx;
		  maxy = (y2>maxy)?y2:maxy;
		  maxz = (z2>maxz)?z2:maxz;
		  if (maxx-minx>bbsize)
			  bbsize = (maxx-minx);
		  if (maxy-miny>bbsize)
			  bbsize = (maxy-miny);
		  if (maxz-minz>bbsize)
			  bbsize = (maxz-minz);


          // store first interpolated vertex of triangle
          vertex[0].point[0] = x1 + interpol * (x2 - x1);
          vertex[0].point[1] = y1 + interpol * (y2 - y1);
          vertex[0].point[2] = z1 + interpol * (z2 - z1);
          
          // get second points of each frame
          x1 = vList[triIndex[i].meshIndex[2]].point[0];
          y1 = vList[triIndex[i].meshIndex[2]].point[1];
          z1 = vList[triIndex[i].meshIndex[2]].point[2];
          x2 = nextVList[triIndex[i].meshIndex[2]].point[0];
          y2 = nextVList[triIndex[i].meshIndex[2]].point[1];
          z2 = nextVList[triIndex[i].meshIndex[2]].point[2];
          
          // store second interpolated vertex of triangle
          vertex[2].point[0] = x1 + interpol * (x2 - x1);
          vertex[2].point[1] = y1 + interpol * (y2 - y1);
          vertex[2].point[2] = z1 + interpol * (z2 - z1);   
          
          // get third points of each frame
          x1 = vList[triIndex[i].meshIndex[1]].point[0];
          y1 = vList[triIndex[i].meshIndex[1]].point[1];
          z1 = vList[triIndex[i].meshIndex[1]].point[2];
          x2 = nextVList[triIndex[i].meshIndex[1]].point[0];
          y2 = nextVList[triIndex[i].meshIndex[1]].point[1];
          z2 = nextVList[triIndex[i].meshIndex[1]].point[2];
     

          // store third interpolated vertex of triangle
          vertex[1].point[0] = x1 + interpol * (x2 - x1);
          vertex[1].point[1] = y1 + interpol * (y2 - y1);
          vertex[1].point[2] = z1 + interpol * (z2 - z1);
          
          // calculate the normal of the triangle
          CalculateNormal(vertex[0].point, vertex[2].point, vertex[1].point);
          
          // render properly textured triangle
          glTexCoord2f(st[triIndex[i].stIndex[0]].s,
               st[triIndex[i].stIndex[0]].t);
          glVertex3fv(vertex[0].point);
          
          glTexCoord2f(st[triIndex[i].stIndex[2]].s ,
               st[triIndex[i].stIndex[2]].t);
          glVertex3fv(vertex[2].point);
          
          glTexCoord2f(st[triIndex[i].stIndex[1]].s,
               st[triIndex[i].stIndex[1]].t);
          glVertex3fv(vertex[1].point);
     }
     glEnd();
     
     glPopMatrix();

     interpol += percent;  // increase percentage of interpolation between frames
     
     glBindTexture( GL_TEXTURE_2D, 0); 
     
     
     if (box) 
     {
		 glLineWidth(2.0);
		 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		 glColor3f( 1.0, 0, 0);
		 DrawBox(true);
	 }
     
     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
     
     if (childNode!=NULL) childNode->AnimateModel(startFrame, endFrame, percent);
     
     return 0;
}

void CModelMd2::Draw(bool box)
{
     vector_t *vList;
     float x1, y1, z1; 
     int i;
     
     if (currentFrame >= maxFrames) return;
         
     // create a pointer to the frame we want to show
     vList = &vertexList[numVertices * currentFrame];
     
     // display the textured model with lighting normals
      	  
	  glBindTexture( GL_TEXTURE_2D, TextureID); 
	  
	  if (TextureID==0) 
      {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(1.0);	
	  }
	        	

	  glPushMatrix();
	 
       glTranslatef(Position.x, Position.y, Position.z);
        glRotatef( Rotation.x, 1, 0, 0 );   // rotation around x-axis
	    glRotatef( Rotation.y, 0, 1, 0 );   // rotation around y-axis
	    glRotatef( Rotation.z, 0, 0, 1 );   // rotation around z-axis
        glScalef( Scale, Scale, Scale);
       /*
	   glBegin(GL_POINTS);
		
		for (i = 0; i < numVertices; i++)
		{
		  glVertex3f( vList[i].point[0] , vList[i].point[1], vList[i].point[2]);
		}
	   glEnd();
	   * */
	   
	    minx = 100000;
		maxx= -100000;
		miny = 100000;
		maxy= -100000;
		minz = 100000;
		maxz= -100000;
		bbsize = 0.0;
	   
	   
	   glColor3f( 1.0f, 1.0f, 1.0f);
	   glBegin(GL_TRIANGLES);
	   for(i = 0; i < numTriangles; i++) // numTriangles
       {
		   
		  x1 = vList[triIndex[i].meshIndex[0]].point[0];
          y1 = vList[triIndex[i].meshIndex[0]].point[1];
          z1 = vList[triIndex[i].meshIndex[0]].point[2];
          
          
          minx = (x1<minx)?x1:minx;
		  miny = (y1<miny)?y1:miny;
		  minz = (z1<minz)?z1:minz;
		  maxx = (x1>maxx)?x1:maxx;
		  maxy = (y1>maxy)?y1:maxy;
		  maxz = (z1>maxz)?z1:maxz;
		  if (maxx-minx>bbsize)
			  bbsize = (maxx-minx);
		  if (maxy-miny>bbsize)
			  bbsize = (maxy-miny);
		  if (maxz-minz>bbsize)
			  bbsize = (maxz-minz);
          
          
		  
		  CalculateNormal(vList[triIndex[i].meshIndex[0]].point,
               vList[triIndex[i].meshIndex[2]].point,
               vList[triIndex[i].meshIndex[1]].point);
		   
		  glTexCoord2f(st[triIndex[i].stIndex[0]].s, st[triIndex[i].stIndex[0]].t); 
		   glVertex3fv(vList[triIndex[i].meshIndex[0]].point); 
		  glTexCoord2f(st[triIndex[i].stIndex[2]].s, st[triIndex[i].stIndex[2]].t); 
		   glVertex3fv(vList[triIndex[i].meshIndex[2]].point);
		  glTexCoord2f(st[triIndex[i].stIndex[1]].s, st[triIndex[i].stIndex[1]].t); 
		   glVertex3fv(vList[triIndex[i].meshIndex[1]].point);  
		   
	   
	   }
	    glEnd();
	  glPopMatrix();
     
     glBindTexture( GL_TEXTURE_2D, 0); 
     
     if (box) 
     {
		 glLineWidth(2.0);
		 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		 glColor3f( 1.0, 0, 0);
		 DrawBox(true);
	 }
     glPolygonMode( GL_FRONT_AND_BACK, GL_FILL); 
     
     if (childNode!=NULL) childNode->Draw();     	
     
}

// CalculateNormal()
// desc: given 3 points, calculates the normal to the points
void CModelMd2::CalculateNormal( float *p1, float *p2, float *p3 )
{
   float a[3], b[3], result[3];
   float length;

   a[0] = p1[0] - p2[0];
   a[1] = p1[1] - p2[1];
   a[2] = p1[2] - p2[2];

   b[0] = p1[0] - p3[0];
   b[1] = p1[1] - p3[1];
   b[2] = p1[2] - p3[2];

   result[0] = a[1] * b[2] - b[1] * a[2];
   result[1] = b[0] * a[2] - a[0] * b[2];
   result[2] = a[0] * b[1] - b[0] * a[1];

   // calculate the length of the normal
   length = (float)sqrt(result[0]*result[0] + result[1]*result[1] + result[2]*result[2]);

   // normalize and specify the normal
   glNormal3f(result[0]/length, result[1]/length, result[2]/length);
}


CModelMd2* CModelMd2::clone()
{
	// create new model
	CModelMd2* m = new CModelMd2();
	
	m->minx=minx; m->maxx=maxx; m->miny=miny; m->maxy=maxy; m->minz=minz; m->maxz=maxz;
	m->Scale=Scale; 
	m->ScaleBB=ScaleBB;
	m->TextureID=TextureID; 
	
	m->SetColor(Color);
	m->filled=filled;
	m->currentFrame=0;
	m->nextFrame=1;
	m->maxFrames=maxFrames;
	m->numFrames=numFrames;
	m->interpol=0;
	
	
	if (childNode != NULL) m->childNode=childNode->clone();
	else m->childNode=NULL;
	
	// copy vertex data data
	if (numVertices > 0)
	{
		
		m->numVertices=numVertices;
		m->numTriangles=numTriangles;
	    m->numST=numST;
	    
	    //cout << "cloning" << endl;
	    //cout << m->numVertices << " " << m->numTriangles << " " << m->numST << endl;
	   
		m->triIndex = new mesh_t[numTriangles];
	    m->st = new texCoord_t[numST];
	    m->vertexList = new vector_t[numVertices*numFrames];
	    
	    memcpy( m->triIndex, triIndex, numTriangles*sizeof( mesh_t));
	    memcpy( m->vertexList, vertexList, numVertices*numFrames*sizeof( vector_t));
	    memcpy( m->st, st, numST*sizeof( texCoord_t)); 
	}
	
	return (CModelMd2*)m;
}

// Unload()
// desc: unloads model data from memory
void CModelMd2::clear()
{
     if (triIndex != NULL)
          delete [] triIndex;
     if (vertexList != NULL)
          delete [] vertexList;
     if (st != NULL)
          delete [] st; 

}


