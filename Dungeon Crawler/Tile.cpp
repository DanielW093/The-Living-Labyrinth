#include "Tile.h"
#include <time.h>
#include <Game.h>
#include "MyGame.h"
#include "World.h"

Tile::Tile(float x, float z, int rotation, CModel* fm,CModel* sWall,CModel* cor1,CModel* cor2,CModel* pil)
{
	tX = x;
	tZ = z;
	tRotation = rotation;
	//Tile Floor
	fm->SetPosition(x, z);
	Floor = fm;
	straightWallModel = sWall;
	corner1Model = cor1;
	corner2Model = cor2;
	pillarModel = pil;

	openSides[0] = 0;
	openSides[1] = 0;
	openSides[2] = 0;
	openSides[3] = 0;
}


Tile::~Tile(void)
{
}

float Tile::distance(float x,float z,float x2,float z2)
{
	return sqrt(pow(x-x2,2)+pow(z-z2,2));
}

void Tile::updateTile(Uint32 t)
{
	for each (CModel *pWall in walls)
	{
		pWall->Update(t);
	}

	Floor->Update(t);
}

void Tile::render3DTile()
{
	for each (CModel *pWall in walls)
	{
		pWall->Draw();
	}
	
	Floor->Draw();
}

void Tile::createEndHall()
{
	tileType = "EndHall";
	//New Models
	CModel* SW1 = straightWallModel->clone();
	CModel* SW2 = straightWallModel->clone();
	CModel* SW3 = straightWallModel->clone();

	//Set correct rotation and position
	switch (tRotation)
	{
		case 0: 
			SW1->SetRotation(0);
			SW2->SetRotation(180);
			SW3->SetRotation(90);
			SW1->SetPosition(tX - 450, tZ);
			SW2->SetPosition(tX + 450, tZ);
			SW3->SetPosition(tX, tZ + 450);
			break;
		case 90:
			SW1->SetRotation(90);
			SW2->SetRotation(270);
			SW3->SetRotation(0);
			SW1->SetPosition(tX, tZ + 450);
			SW2->SetPosition(tX, tZ - 450);
			SW3->SetPosition(tX - 450, tZ);
			break;
		case 180:
			SW1->SetRotation(0);
			SW2->SetRotation(180);
			SW3->SetRotation(270);
			SW1->SetPosition(tX - 450, tZ);
			SW2->SetPosition(tX + 450, tZ);
			SW3->SetPosition(tX, tZ - 450);
			break;
		case 270:
			SW1->SetRotation(90);
			SW2->SetRotation(270);
			SW3->SetRotation(180);
			SW1->SetPosition(tX, tZ + 450);
			SW2->SetPosition(tX, tZ - 450);
			SW3->SetPosition(tX + 450, tZ);
			break;
	}
	//Push into vector
	walls.push_back(SW1);
	walls.push_back(SW2);
	walls.push_back(SW3);
}

void Tile::createStraightHall()
{
	tileType = "StraightHall";
	//New Models
	CModel* SW1 = straightWallModel->clone();
	CModel* SW2 = straightWallModel->clone();
	//Set correct rotation and position
	switch (tRotation)
	{
		case 0: 
			SW1->SetRotation(0);
			SW2->SetRotation(180);
			SW1->SetPosition(tX - 450, tZ);
			SW2->SetPosition(tX + 450, tZ);
			openSides[0] = 1;
			openSides[2] = 1;
			break;
		case 90:
			SW1->SetRotation(90);
			SW2->SetRotation(270);
			SW1->SetPosition(tX, tZ + 450);
			SW2->SetPosition(tX, tZ - 450);
			openSides[1] = 1;
			openSides[3] = 1;
			break;
	}
	//Push into vector
	walls.push_back(SW1);
	walls.push_back(SW2);
}

void Tile::createCornerHall()
{
	tileType = "CornerHall";
	//New Models
	CModel* CW1 = corner1Model->clone();
	CModel* CW2 = corner2Model->clone();
	CModel* CW3 = pillarModel->clone();
	//Set correct rotation and position
	switch (tRotation)
	{
		case 0:
			CW1->SetRotation(tRotation);
			CW2->SetRotation(tRotation);
			CW3->SetRotation(tRotation);
			CW1->SetPosition(tX, tZ - 449.64);
			CW2->SetPosition(tX - 450, tZ);
			CW3->SetPosition(tX + 450, tZ + 450);
			openSides[1] = 1;
			openSides[2] = 1;
			break;
		case 90:
			CW1->SetRotation(tRotation);
			CW2->SetRotation(tRotation);
			CW3->SetRotation(tRotation);
			CW1->SetPosition(tX - 449.64, tZ);
			CW2->SetPosition(tX, tZ + 450);
			CW3->SetPosition(tX + 450, tZ - 450);
			openSides[0] = 1;
			openSides[1] = 1;
			break;
		case 180:
			CW1->SetRotation(tRotation);
			CW2->SetRotation(tRotation);
			CW3->SetRotation(tRotation);
			CW1->SetPosition(tX, tZ + 449.64);
			CW2->SetPosition(tX + 450, tZ);
			CW3->SetPosition(tX - 450, tZ - 450);
			openSides[0] = 1;
			openSides[3] = 1;
			break;
		case 270:
			CW1->SetRotation(tRotation);
			CW2->SetRotation(tRotation);
			CW3->SetRotation(tRotation);
			CW1->SetPosition(tX + 449.64, tZ);
			CW2->SetPosition(tX, tZ - 450);
			CW3->SetPosition(tX - 450, tZ + 450);
			openSides[2] = 1;
			openSides[3] = 1;
			break;
	}
	//Push into vector
	walls.push_back(CW1);
	walls.push_back(CW2);	
	walls.push_back(CW3);
}

void Tile::createCrossHall()
{
	tileType = "CrossHall";
	//New Models
	CModel* CW1 = pillarModel->clone();
	CModel* CW2 = pillarModel->clone();
	CModel* CW3 = pillarModel->clone();
	CModel* CW4 = pillarModel->clone();
	//Set Position
	CW1->SetPosition(tX + 450, tZ + 450);
	CW2->SetPosition(tX - 450, tZ + 450);
	CW3->SetPosition(tX - 450, tZ - 450);
	CW4->SetPosition(tX + 450, tZ - 450);	
	//Push into vector
	walls.push_back(CW1);
	walls.push_back(CW2);	
	walls.push_back(CW3);
	walls.push_back(CW4);
	//Set Open Sides
	openSides[0] = 1;
	openSides[1] = 1;
	openSides[2] = 1;
	openSides[3] = 1;
}

void Tile::createTHall()
{
	tileType = "THall";
	//New Models
	CModel* CW1 = pillarModel->clone();
	CModel* CW2 = pillarModel->clone();
	CModel* SW1 = straightWallModel->clone();
	//Set correct rotation and position
	switch (tRotation)
	{
		case 0:
			SW1->SetPosition(tX, tZ - 450);
			SW1->SetRotation(270);
			CW1->SetPosition(tX - 450, tZ + 450);
			CW2->SetPosition(tX + 450, tZ + 450);
			openSides[1] = 1;
			openSides[2] = 1;
			openSides[3] = 1;
			break;
		case 90:
			SW1->SetPosition(tX-450, tZ);
			SW1->SetRotation(0);
			CW1->SetPosition(tX + 450, tZ + 450);
			CW2->SetPosition(tX + 450, tZ - 450);
			openSides[0] = 1;
			openSides[1] = 1;
			openSides[2] = 1;
			break;
		case 180:
			SW1->SetPosition(tX, tZ + 450);
			SW1->SetRotation(90);
			CW1->SetPosition(tX - 450, tZ - 450);
			CW2->SetPosition(tX + 450, tZ - 450);
			openSides[0] = 1;
			openSides[1] = 1;
			openSides[3] = 1;
			break;
		case 270:
			SW1->SetPosition(tX+450, tZ);
			SW1->SetRotation(180);
			CW1->SetPosition(tX - 450, tZ + 450);
			CW2->SetPosition(tX - 450, tZ - 450);
			openSides[0] = 1;
			openSides[2] = 1;
			openSides[3] = 1;
			break;
	}
	//Push into vector
	walls.push_back(CW1);
	walls.push_back(CW2);
	walls.push_back(SW1);
	//Set Open Sides
}

void Tile::createSpawnCorner()
{
	tileType = "SpawnCorner";
	//New Models
	CModel* CW1 = corner1Model->clone();
	CModel* CW2 = corner2Model->clone();
	//Set correct rotation and position
	switch (tRotation)
	{
		case 0:
			CW1->SetRotation(tRotation);
			CW2->SetRotation(tRotation);
			CW1->SetPosition(tX, tZ - 449.64);
			CW2->SetPosition(tX - 450, tZ);
			openSides[1] = 1;
			openSides[2] = 1;
			break;
		case 90:
			CW1->SetRotation(tRotation);
			CW2->SetRotation(tRotation);
			CW1->SetPosition(tX - 449.64, tZ);
			CW2->SetPosition(tX, tZ + 450);
			openSides[0] = 1;
			openSides[1] = 1;
			break;
		case 180:
			CW1->SetRotation(tRotation);
			CW2->SetRotation(tRotation);
			CW1->SetPosition(tX, tZ + 449.64);
			CW2->SetPosition(tX + 450, tZ);
			openSides[0] = 1;
			openSides[3] = 1;
			break;
		case 270:
			CW1->SetRotation(tRotation);
			CW2->SetRotation(tRotation);
			CW1->SetPosition(tX + 449.64, tZ);
			CW2->SetPosition(tX, tZ - 450);
			openSides[2] = 1;
			openSides[3] = 1;
			break;
	}
	//Push into vector
	walls.push_back(CW1);
	walls.push_back(CW2);	
}

bool Tile::isPlayerInside()
{
	return playerInside;
}

float Tile::GetTileX()
{
	if(tX != NULL)
	{
		return tX;
	}
	else
	{
		return 0;
	}
}

float Tile::GetTileZ()
{
	return tZ;
}

int Tile::GetTileRotation()
{
	return tRotation;
}

string Tile::getTileType()
{
	return tileType;
}

vector<CModel*> Tile::getWalls()
{
	return walls;
}

void Tile::SetTileX(float x)
{
	tX = x;
}

void Tile::SetTileZ(float z)
{
	tZ = z;
}

void Tile::SetTileRotation(int r)
{
	tRotation = r;
}