#pragma once

#include <string>
#include "Game.h"
#include <vector>

class Tile
{
private:
	float tX,tZ; //Center of tile x,z
	int tRotation; //Rotation of tile
	CModel* Floor;
	CModel* straightWallModel;
	CModel* corner1Model;
	CModel* corner2Model;
	CModel* pillarModel;
	CModel* chestModel;
	string tileType;
	bool playerInside;
	int spawnChest;
	
public:

	vector<CModel*> walls; //Vector containing wall models
	int openSides[4]; //0 = Up , 1 = Right, 2 = Down, 3 = Left
	float playerDistance;

	Tile(float x, float z, int rotation, CModel* floorModel,CModel* straightWall,CModel* corner1,CModel* corner2,CModel* pillar);
	~Tile(void);

	//Game loops calls
	void updateTile(Uint32 t);
	void render3DTile();

	//TODO: player hittests

	float distance(float x, float z,float x2,float z2);
	//Creating tiles
	void createEndHall();
	void createStraightHall();
	void createCornerHall();
	void createCrossHall();
	void createTHall();

	void createSpawnCorner();

	//Getters
	float GetTileX();
	float GetTileZ();
	int GetTileRotation();
	string getTileType();
	bool isPlayerInside();

	vector<CModel*> getWalls();

	//Setters
	void SetTileX(float x);
	void SetTileZ(float z);
	void SetTileRotation(int r);

};

