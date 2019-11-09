#pragma once
#include <vector>
#include "Tile.h"
#include "Chest.h"
#include "Enemy.h"

class World
{
private:
	
public:

	vector<Tile*> tiles;
	vector <Enemy*> enemies;

	World(void);
	~World(void); 

	vector<Chest*> chests;

	void updateWorld(Uint32 t, World world, float playerX, float playerZ);
	void render3DWorld();
	void setWorld(World w);
};

