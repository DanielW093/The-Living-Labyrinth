#include "Model.h"

#include <iostream>
#include <fstream>

using namespace std;


// Update function
void CModel::Update(Uint32 GameTime)
{
	if (SpriteTime == 0) { SpriteTime = GameTime; return; }
	
	if (childNode!=NULL) CNodeUpdate();

	int deltaTime = GameTime - SpriteTime;
	if ( deltaTime < 0) deltaTime = 0;

	// linear motion update
	
    float dx = float(Velocity*Direction.x*deltaTime/1000.0);
	float dy = float(Velocity*Direction.y*deltaTime/1000.0);
	float dz = float(Velocity*Direction.z*deltaTime/1000.0);

    Position.x += dx; Position.y += dy; Position.z += dz;
	
	// rotation update
	Rotate( Omega.x*deltaTime/1000.0f, Omega.y*deltaTime/1000.0f, Omega.z*deltaTime/1000.0f);
	
    SpriteTime = GameTime;
    
    if (childNode!=NULL) { childNode->Update( GameTime); }
		 		
}


void CModel::CNodeUpdate()
{
	
	 if (childNode!=NULL)
	 {
		childNode->SetPositionV( GetPositionV());
		childNode->SetDirectionV( GetDirectionV());
		childNode->Velocity=Velocity;
		childNode->Rotation.x=Rotation.x; childNode->Rotation.y=Rotation.y; childNode->Rotation.z=Rotation.z;
		childNode->Omega.x=Omega.x; childNode->Omega.y=Omega.y; childNode->Omega.z=Omega.z;
		
		childNode->currentFrame=currentFrame;
	 }
}

bool CModel::LoadModel(string filename)
{
	ifstream inFile;
	string str;   // string containing a line of text
	int numV,numT,numN;
	
	numVertices=0;
	
	cout << "loading... " << filename; 
	
	inFile.open(filename.c_str(), ios::in);
	
	// ---- determining array sizes ------
	if (inFile.is_open())
	{
	
	  while (!inFile.eof())
	  {
		  getline( inFile, str); // read a line of text into string str
		  if (str.length() < 1) continue;
		  if (str[0]=='f' && str[1]==' ') numVertices=numVertices+3;
		  else {}  	  
	  }
	  //cout << " Vertices: " << numVertices << endl;
	  inFile.close();	
	}
	else 
	{
		cout << " fail" << endl;
		return false;
	}
	
	if (numVertices >= 9999)
	{
		cout << " FAIL: The model exceeds vertex limit" << endl;
		numVertices=0;
		inFile.close();	
		return false;
	}
	
	// ---- allocating the necessary memory ------
	vertexArray = new vector3f[numVertices];
	normalArray = new vector3f[numVertices];
	texArray = new vector2f[numVertices];
	vector3f* tmpArrayV=new vector3f[numVertices];
	vector2f* tmpArrayT=new vector2f[numVertices];
	vector3f* tmpArrayN=new vector3f[numVertices];


	numVertices=0;
	numT=numN=numV=0;
	
	minx = 100000; maxx= -100000;
	miny = 100000; maxy= -100000;
	minz = 100000; maxz= -100000;
	
	
	inFile.open(filename.c_str(), ios::in);
	
	// ---- reading in vertex and texture data ------
	if (inFile.is_open())
	{
	  while (!inFile.eof())
	  {
		  getline( inFile, str); // read a line of text into string str
		  if (str.length() < 1) continue;
		  if (str[0]=='v' && str[1]==' ')
		  {
			 str[0]=' ';
			 sscanf(str.c_str(),"%f %f %f", &tmpArrayV[numV].x, &tmpArrayV[numV].y, &tmpArrayV[numV].z);
			 
			 minx = (tmpArrayV[numV].x<minx)?tmpArrayV[numV].x:minx;
			 miny = (tmpArrayV[numV].y<miny)?tmpArrayV[numV].y:miny;
		     minz = (tmpArrayV[numV].z<minz)?tmpArrayV[numV].z:minz;
		     maxx = (tmpArrayV[numV].x>maxx)?tmpArrayV[numV].x:maxx;
		     maxy = (tmpArrayV[numV].y>maxy)?tmpArrayV[numV].y:maxy;
		     maxz = (tmpArrayV[numV].z>maxz)?tmpArrayV[numV].z:maxz;
			 
			 numV++; 
			 //cout << str<< endl;
		  }	
		  else if (str[0]=='v' && str[1]=='t')
		  {
			 str[0]=' '; str[1]=' ';
			 sscanf(str.c_str(),"%f %f", &tmpArrayT[numT].x, &tmpArrayT[numT].y); 
			 numT++;
		     //cout << str<< endl;
		  }
		  else if (str[0]=='v' && str[1]=='n')
		  {
			 str[0]=' '; str[1]=' ';
			 sscanf(str.c_str(),"%f %f %f", &tmpArrayN[numN].x, &tmpArrayN[numN].y, &tmpArrayN[numN].z); 
			 // todo: normalize normal vector
			 normalizeVector3f(  &tmpArrayN[numN]);
			 numN++; 
			 //cout << str<< endl;
		  }	
		  else if (str[0]=='f' && str[1]==' ')
		  {
			  
			  if (!readFace( str, tmpArrayV, tmpArrayN, tmpArrayT))
			  {
				 cout << " FAIL: This model is not triangulated. To triangulate, load it into Misfit 3D and save it again." << endl;
				 inFile.close();
				 return false;	  
			  }
		  }
		  else {} 
	  } 
	  cout << "..ok";
	  cout << " Triang:" << numV/3 << " X:" << int(maxx+minx) << " Y:" << int(maxy-miny) << " Z:" << int(maxz-minz)  << endl;
		
	  inFile.close();	
	}
	
	// todo: calculate normals from vertex array
	
	if (numN == 0) CalculateNormals();
	

	delete [] tmpArrayV;
	delete [] tmpArrayN;
	delete [] tmpArrayT;
	
	return true;
}


void CModel::CalculateNormals()
{
   vector3f a, b, c;
   float length;
   
   
   for (int n=0; n < numVertices; n=n+3)
   {
	   
     a.x = vertexArray[n].x - vertexArray[n+1].x;
     a.y = vertexArray[n].y - vertexArray[n+1].y;
     a.z = vertexArray[n].z - vertexArray[n+1].z;

     b.x = vertexArray[n].x - vertexArray[n+2].x;
     b.y = vertexArray[n].y - vertexArray[n+2].y;
     b.z = vertexArray[n].z - vertexArray[n+2].z;

     c.x = a.y * b.z - b.y * a.z;
     c.y = b.x * a.z - a.x * b.z;
     c.z = a.x * b.y - b.x * a.y;

     // calculate the length of the normal
     length = (float)sqrt(c.x*c.x + c.y*c.y + c.z*c.z);
     
     normalArray[n].x=c.x/length;  normalArray[n].y=c.y/length;   normalArray[n].z=c.z/length; 
     normalArray[n+1].x=c.x/length;  normalArray[n+1].y=c.y/length;   normalArray[n+1].z=c.z/length; 
     normalArray[n+2].x=c.x/length;  normalArray[n+2].y=c.y/length;   normalArray[n+2].z=c.z/length; 
     
   }
}


void CModel::clear()
{
		
	  if (vertexArray) delete [] vertexArray;
	  if (normalArray) delete [] normalArray;
	  if (texArray) delete [] texArray;	
	  
	  texArray=NULL; vertexArray=NULL; texArray=NULL;
	  numVertices=0;
	  
}


bool CModel::readFace(string& str, vector3f* tmpArrayV, vector3f* tmpArrayN, vector2f* tmpArrayT)
{
	int num[4];
	int values[9]; // up to 9 values to be read in
	int numNum=0;
	unsigned int n;
	int numVertex=0;
	int value=0;
	
	// clear values
	memset(values,0,9*sizeof(int));
	
	
	for (n=1; n < str.length(); n++)
	{
		
		if (str[n]==' ' && numVertex==0) { numVertex++; continue; } // start first vertex
		
		if (str[n] >=48 && str[n] <= 57)  // read numbers as characters
		{
		  num[numNum++]=str[n]-48; 
		  if  (n < str.length()-1) continue;
		}
		
		if (str[n] == '/' || str[n]==' ' || n == str.length()-1) 
		{
		  if (numNum==1) { value=num[0];  }
		  else if (numNum==2) { value=num[1]+10*num[0];}
		  else if (numNum==3) { value=num[2]+10*num[1]+100*num[0];}
		  else if (numNum==4) { value=num[3]+10*num[2]+100*num[1]+1000*num[0];}
		  else value=0; // no value
		  values[numVertex-1]=value; // assign value
		  numNum=0; numVertex++; 
		 
		}
	}
	
	// this model is not triangulated
	if (numVertex > 10) return false;
	
	if (numVertex > 7)  // v/t/n 
	{
     memcpy( &vertexArray[ numVertices], &tmpArrayV[values[0]-1],sizeof( vector3f));
	 if (values[2]>0) memcpy( &normalArray[ numVertices], &tmpArrayN[values[2]-1],sizeof( vector3f));
	 if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[1]-1],sizeof( vector2f));	
	 numVertices++;
	
	 memcpy( &vertexArray[ numVertices], &tmpArrayV[values[3]-1],sizeof( vector3f));
	 if (values[2]>0) memcpy( &normalArray[ numVertices], &tmpArrayN[values[5]-1],sizeof( vector3f));
	 if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[4]-1],sizeof( vector2f)); 	
     numVertices++;
    
     memcpy( &vertexArray[ numVertices], &tmpArrayV[values[6]-1],sizeof( vector3f));
     if (values[2]>0) memcpy( &normalArray[ numVertices], &tmpArrayN[values[8]-1],sizeof( vector3f));
     if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[7]-1],sizeof( vector2f));
	 numVertices++;
    }
    else  // v/t
    {
	 memcpy( &vertexArray[ numVertices], &tmpArrayV[values[0]-1],sizeof( vector3f));
	 if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[1]-1],sizeof( vector2f));	
	 numVertices++;
	
	 memcpy( &vertexArray[ numVertices], &tmpArrayV[values[2]-1],sizeof( vector3f));
	 if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[3]-1],sizeof( vector2f)); 	
     numVertices++;
    
     memcpy( &vertexArray[ numVertices], &tmpArrayV[values[4]-1],sizeof( vector3f));
     if (values[1]>0) memcpy( &texArray[ numVertices], &tmpArrayT[values[5]-1],sizeof( vector2f));
	 numVertices++;
	}
	return true;
}


void CModel::Draw(bool box)
{
	if (numVertices > 0 && vertexArray != NULL)
	{
	  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	  glColor3f(1,1,1);
	  // binding texture to handle
      glBindTexture( GL_TEXTURE_2D, TextureID);
      
      glPushMatrix();
	   glTranslatef( Position.x, Position.y, Position.z);  	// transformation to world coordinates
	   glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
	   glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
	   glRotatef( Rotation.z, 0, 0, 1 );       // rotation around z-axis
       glScalef( Scale, Scale, Scale);
		
		glEnableClientState(GL_VERTEX_ARRAY);	 
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 		
		glTexCoordPointer(2, GL_FLOAT, 0, texArray);			
		glVertexPointer(3,GL_FLOAT,	0, vertexArray);				
		glNormalPointer(GL_FLOAT, 0, normalArray);
	
							
		glDrawArrays(GL_TRIANGLES, 0, numVertices);		
		glDisableClientState(GL_VERTEX_ARRAY);						
		glDisableClientState(GL_NORMAL_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	  
	  glPopMatrix();
	  
	  glBindTexture( GL_TEXTURE_2D, 0);
	  	
	}
	else
	{
	  glLineWidth(2.0);
	  glColor4f( Color.R, Color.G, Color.B, Color.A);
	  if (!filled) glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	  else glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	  DrawBox(false);
    }
	
	if (box)
	{
		 glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
		 glColor3f( 1.0, 0, 0);
		 DrawBox(true);
	}
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	
	
	if (childNode!=NULL) Draw( box);
		
}

void CModel::DrawBox(bool bbox)
{
    	glPushMatrix();
		  glTranslatef( Position.x, Position.y, Position.z);  	// transformation to world coordinates
		  glRotatef( Rotation.x, 1, 0, 0 );       // rotation around x-axis
		  glRotatef( Rotation.y, 0, 1, 0 );       // rotation around y-axis
		  glRotatef( Rotation.z, 0, 0, 1 );       // rotation around z-axis
          
          if (bbox) glScalef( ScaleBB*Scale, ScaleBB*Scale, ScaleBB*Scale);
          else glScalef( Scale, Scale, Scale);
          
		  glBegin(GL_QUADS);
		  
		   // front
		   glNormal3f(0,0,1);
		   glVertex3f(minx, miny,  maxz);
		   glVertex3f(maxx, miny,  maxz);
		   glVertex3f(maxx, maxy,  maxz);
		   glVertex3f(minx, maxy,  maxz);
		   // back
		   glNormal3f(0,0,-1);
		   glVertex3f(minx, miny,  minz);
		   glVertex3f(maxx, miny,  minz);
		   glVertex3f(maxx, maxy,  minz);
		   glVertex3f(minx, maxy,  minz);
		   // top
		   glNormal3f(0,1,0);
		   glVertex3f(minx, maxy,  maxz);
		   glVertex3f(maxx, maxy,  maxz);
		   glVertex3f(maxx, maxy,  minz);
		   glVertex3f(minx, maxy,  minz);
		   // bottom
		   glNormal3f(0,-1,0);
		   glVertex3f(minx, miny,  maxz);
		   glVertex3f(maxx, miny,  maxz);
		   glVertex3f(maxx, miny,  minz);
		   glVertex3f(minx, miny,  minz);
		   // left
		   glNormal3f(-1,0,0);
		   glVertex3f(minx, maxy,  maxz);
		   glVertex3f(minx, miny,  maxz);
		   glVertex3f(minx, miny,  minz);
		   glVertex3f(minx, maxy,  minz);
		   // right
		    glNormal3f(1,0,0);
		   glVertex3f(maxx, maxy,  maxz);
		   glVertex3f(maxx, miny,  maxz);
		   glVertex3f(maxx, miny,  minz);
		   glVertex3f(maxx, maxy,  minz);
		   
		  glEnd();
		glPopMatrix();	
}



// move in xz plane according to rotation angle
void CModel::MoveXZRotation( float distance) 
{  
  Position.x += float(distance*cos( DEG2RAD (Rotation.y))); Position.z -= float(distance*sin( DEG2RAD (Rotation.y))); 
}

// move in xyz plane according to specified direction
void CModel::Move( float distance) 
{  
	Position.x += distance*Direction.x;
	Position.y += distance*Direction.y;
	Position.z += distance*Direction.z; 
}

void CModel::DrawVertex( float x, float y, float z)
{
	glColor3f(1.0,1.0,0);
	glPointSize( 10); 
	
	glBegin(GL_POINTS);
	 glVertex3f( x, y, z);
	glEnd();
		
}


CModel* CModel::clone()
{
	// create new model
	CModel* m = new CModel();
	
	m->minx=minx; m->maxx=maxx; m->miny=miny; m->maxy=maxy; m->minz=minz; m->maxz=maxz;
	
	m->Scale=Scale; 
	m->ScaleBB=ScaleBB;
	m->TextureID=TextureID; 
	
	m->SetColor(Color);
	m->filled=filled;
	
	if (childNode != NULL) m->childNode=childNode->clone();
	else m->childNode=NULL;
	
	// copy vertex data data
	if (numVertices > 0)
	{
		m->numVertices=numVertices;
		m->vertexArray = new vector3f[numVertices];
	    m->normalArray = new vector3f[numVertices];
	    m->texArray = new vector2f[numVertices];
	    
	    memcpy( m->vertexArray, vertexArray, numVertices*sizeof( vector3f));
	    memcpy( m->normalArray, normalArray, numVertices*sizeof( vector3f));
	    memcpy( m->texArray, texArray, numVertices*sizeof( vector2f)); 
	   
	}
	
	return m;
}

double CModel::GetRadius()
{
	double xmax, ymax, zmax;
	
	if (abs(minx) > maxx) xmax=abs(minx);
	else xmax=maxx;
	
	if (abs(miny) > maxy) ymax=abs(miny);
	else ymax=maxy;
	
	if (abs(minz) > maxz) zmax=abs(minz);
	else zmax=maxz;
	
	return Scale*sqrt( xmax*xmax+ymax*ymax+zmax*zmax);
}

bool CModel::HitTest(vector3f& pos, float distance)
{
	double dist;
	
	dist=sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
	
	if (dist < distance) return true;
	
	return false;
}

bool CModel::HitTest(vector3f& pos)
{
	double xp,yp,zp;
	double x,y,z;
	
	x=xp=pos.x-Position.x; y=yp=pos.y-Position.y;  z=zp=pos.z-Position.z; 
  
    // rotation around all three axis simultaneously not yet supported
  
    if (int(Rotation.x) == 0)
    {
	 
	  // bounding box rotation around the y-axis
     xp = x*cos(DEG2RAD (-Rotation.y)) + z*sin(DEG2RAD (-Rotation.y));
	 zp = -x*sin(DEG2RAD (-Rotation.y)) + z*cos(DEG2RAD (-Rotation.y));
	 yp = y;
	 
	  x=xp; y=yp;  z=zp;
	  
	 // bounding box rotation around the z-axis
     xp = x*cos(DEG2RAD (-Rotation.z)) - y*sin(DEG2RAD (-Rotation.z));
	 yp = x*sin(DEG2RAD (-Rotation.z)) + y*cos(DEG2RAD (-Rotation.z));
	 zp = z;
	 
    }
    else if (int(Rotation.z)==0)
    {
	 // bounding box rotation around the x-axis
     xp = x;
	 yp = y*cos(DEG2RAD (-Rotation.x)) - z*sin(DEG2RAD (-Rotation.x));
	 zp = y*sin(DEG2RAD (-Rotation.x)) + z*cos(DEG2RAD (-Rotation.x));
	 
	 x=xp; y=yp;  z=zp;
	 
	  // bounding box rotation around the y-axis
     xp = x*cos(DEG2RAD (-Rotation.y)) + z*sin(DEG2RAD (-Rotation.y));
	 zp = -x*sin(DEG2RAD (-Rotation.y)) + z*cos(DEG2RAD (-Rotation.y));
	 yp = y;
	 
    }
    else if (int(Rotation.y)==0)
    {
	 // bounding box rotation around the x-axis
     xp = x;
	 yp = y*cos(DEG2RAD (-Rotation.x)) - z*sin(DEG2RAD (-Rotation.x));
	 zp = y*sin(DEG2RAD (-Rotation.x)) + z*cos(DEG2RAD (-Rotation.x));
	 
	  x=xp; y=yp;  z=zp;	
	 	
	 // bounding box rotation around the z-axis
     xp = x*cos(DEG2RAD (-Rotation.z)) - y*sin(DEG2RAD (-Rotation.z));
	 yp = x*sin(DEG2RAD (-Rotation.z)) + y*cos(DEG2RAD (-Rotation.z));
	 zp = z;
	 
    }
  
    if (xp >= Scale*ScaleBB*minx && xp <= Scale*ScaleBB*maxx && yp >= Scale*ScaleBB*miny && yp <= Scale*ScaleBB*maxy && zp >= Scale*ScaleBB*minz && zp <= Scale*ScaleBB*maxz) 
    {
		return true;
    }
	return false;
}

void CModel::DoRotation( vector3f& pos, float yoffset)
{
	double xp,yp,zp;
	double x,y,z;
	
	x=xp=pos.x-Position.x; y=yp=pos.y-Position.y;  z=zp=pos.z-Position.z; 
	
	// rotation around all three axis simultaneously not yet supported
  
    if (int(Rotation.x) == 0)
    {
	 // correcting for bounding box rotation around the z-axis
     xp = x*cos(DEG2RAD (Rotation.z)) - y*sin(DEG2RAD (Rotation.z));
	 yp = x*sin(DEG2RAD (Rotation.z)) + y*cos(DEG2RAD (Rotation.z));
	 zp = z;
	 
	  x=xp; y=yp;  z=zp;
	  
	  // correcting for bounding box rotation around the y-axis
     xp = x*cos(DEG2RAD (Rotation.y+yoffset)) + z*sin(DEG2RAD (Rotation.y+yoffset));
	 zp = -x*sin(DEG2RAD (Rotation.y+yoffset)) + z*cos(DEG2RAD (Rotation.y+yoffset));
	 yp = y;
    }
    else if (int(Rotation.z)==0)
    {
	  // correcting for bounding box rotation around the y-axis
     xp = x*cos(DEG2RAD (Rotation.y+yoffset)) + z*sin(DEG2RAD (Rotation.y+yoffset));
	 zp = -x*sin(DEG2RAD (Rotation.y+yoffset)) + z*cos(DEG2RAD (Rotation.y+yoffset));
	 yp = y;
	
	 x=xp; y=yp;  z=zp;
    
	 // correcting for bounding box rotation around the x-axis
     xp = x;
	 yp = y*cos(DEG2RAD (Rotation.x)) - z*sin(DEG2RAD (Rotation.x));
	 zp = y*sin(DEG2RAD (Rotation.x)) + z*cos(DEG2RAD (Rotation.x));
    }
    else if (int(Rotation.y)==0)
    {
		
	 // correcting for bounding box rotation around the z-axis
     xp = x*cos(DEG2RAD (Rotation.z)) - y*sin(DEG2RAD (Rotation.z));
	 yp = x*sin(DEG2RAD (Rotation.z)) + y*cos(DEG2RAD (Rotation.z));
	 zp = z;
	 
	  x=xp; y=yp;  z=zp;
	 
	 // correcting for bounding box rotation around the x-axis
     xp = x;
	 yp = y*cos(DEG2RAD (Rotation.x)) - z*sin(DEG2RAD (Rotation.x));
	 zp = y*sin(DEG2RAD (Rotation.x)) + z*cos(DEG2RAD (Rotation.x));
	
	 
    }
    
	pos.x=float(xp+Position.x); pos.y=float(yp+Position.y);  pos.z=float(zp+Position.z); 
	
	//DrawVertex(pos.x, pos.y, pos.z);
}

bool CModel::HitTest(CModel *pModel)
{
   vector3f pos;
   
   pos = pModel->GetPositionV();
   
   double dist;
	
   dist=sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   double radius1=GetRadius(); 
   double radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
  
  
   // --- inside ? -----
   if (pModel->HitTest( Position)) return true;
   
   // --- inside ? -----
   if (HitTest( pModel->GetPositionV())) return true;
  
   // ----  test 8 vertex positions from this model----  
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*minz;
   DoRotation( pos); if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*minz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*minz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*minz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*miny;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*maxx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   pos.x=Position.x+Scale*ScaleBB*minx;  pos.y=Position.y+Scale*ScaleBB*maxy;  pos.z=Position.z+Scale*ScaleBB*maxz; 
   DoRotation( pos);  if (pModel->HitTest( pos)) return true;
   
   
   // ----  test 8 vertex positions from pModel----  
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->minz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->miny;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->maxx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   pos.x=pModel->Position.x+pModel->Scale*ScaleBB*pModel->minx;  pos.y=pModel->Position.y+pModel->Scale*ScaleBB*pModel->maxy;  pos.z=pModel->Position.z+pModel->Scale*ScaleBB*pModel->maxz; 
   pModel-> DoRotation( pos);  if (HitTest( pos)) return true;
   
   return false;
}

bool CModel::HitTestMove(CModel *pModel, float offset)
{
   vector3f pos;
     
   pos = pModel->GetPositionV();
    
   double dist;
	
   dist=sqrt( (pos.x-Position.x)*(pos.x-Position.x)+(pos.y-Position.y)*(pos.y-Position.y)+(pos.z-Position.z)*(pos.z-Position.z));
   
   double radius1=GetRadius(); 
   double radius2=pModel->GetRadius();

   // ----  models are too far apart and could not possibly overlap  ----
   if (dist > radius1+radius2) return false;
   
   // ----  test 9 vertex positions from this model---- 
   
   // --- front middle  
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z;
    DoRotation( pos, offset);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z;
    DoRotation( pos, offset);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z;
    DoRotation( pos, offset);
   if (pModel->HitTest( pos)) return true;
   
   // --- front right
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*maxz;
    DoRotation( pos, offset);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*maxz;
    DoRotation( pos, offset); 
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*maxz;
    DoRotation( pos, offset);
   if (pModel->HitTest( pos)) return true;

   //---- front left
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y; pos.z=Position.z+Scale*ScaleBB*minz;
    DoRotation( pos, offset);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*miny; pos.z=Position.z+Scale*ScaleBB*minz;
    DoRotation( pos, offset);
   if (pModel->HitTest( pos)) return true;
   
   pos.x=Position.x+Scale*ScaleBB*maxx; pos.y=Position.y+Scale*ScaleBB*maxy; pos.z=Position.z+Scale*ScaleBB*minz;
    DoRotation( pos, offset);
   if (pModel->HitTest( pos)) return true;
   
   return false;
	
}









