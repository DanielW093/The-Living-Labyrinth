#pragma once
#include "World.h"
#include "Tile.h"
#include <stdlib.h>
#include <time.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <Windows.h>

class WorldProvider
{

private:
	World world;
	float curX,curZ;
	int calculateRotation(int hallType, int side);
	bool shouldCreate(int x,int z);
	void createChest(float x, float z, string TileType, int tileRotation);

public:
	WorldProvider(void);
	~WorldProvider(void);

	void initialiseWorld();
	void generateWorld(int size, int level);
	Tile* createTile(int a, float x,float z,int rotation);
	Enemy* createEnemy(CModelMd2* model, float x, float z);
	World getWorld();

	CModel straightWall;
	CModel corner1;
	CModel corner2;
	CModel pillar;
	CModel floor;
	CModelMd2 chest;
	CModelMd2 chestLid;
	CModelMd2 enemy;

	CTexture floorTex;
	CTexture wallTex;
	CTexture straightWallTex;
	CTexture corner1Tex;
	CTexture corner2Tex;
	CTexture pillarTex;
	CTexture chestTex;
	CTexture enemyTex;
};

