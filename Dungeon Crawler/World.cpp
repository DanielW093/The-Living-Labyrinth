#include "World.h"


World::World(void)
{
}


World::~World(void)
{
}

void World::updateWorld(Uint32 t, World world, float playerX, float playerZ)
{
	for each(Tile* pTile in tiles)
	{
		pTile->updateTile(t);
	}
	for each(Chest* pChest in chests)
	{	
		pChest->updateChest(t);
	}
	for each (Enemy* pEnemy in enemies)
	{
		pEnemy->updateEnemy(t);
		pEnemy->moveEnemy(playerX, playerZ, world);
		
		if (pEnemy->enemy->GetHealth() <= 0)
		{
			pEnemy->isAlive = false;
		}
	}
}

void World::render3DWorld()
{
	for each(Tile* pTile in tiles)
	{	
		pTile->render3DTile();
	}

	for each(Chest* pChest in chests)
	{
		if (!pChest->getOpen())
		{
			pChest->drawChest();
		}
		else
		{
			pChest->animateChest();
		}
	}

	for each (Enemy* pEnemy in enemies)
	{
		pEnemy->render3DEnemy();
	}
}


void World::setWorld(World w)
{
	tiles = w.tiles;
	chests = w.chests;
	enemies = w.enemies;

	w.tiles.clear();
	w.chests.clear();
	w.enemies.clear();
}

