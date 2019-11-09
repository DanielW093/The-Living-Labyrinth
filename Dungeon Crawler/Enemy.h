#pragma once

#include <Game.h>

class Player; // Trying to avoid endless include loops works but not sure if correct
class World;

class Enemy
{
private:
	bool isMoving;
public:
	Enemy(CModelMd2* model, float x, float z);
	~Enemy(void);

	bool isAlive;
	bool attackCooldown;

	Uint32 cooldownStart;

	CModelMd2* enemy;

	void updateEnemy(Uint32 time);
	void render3DEnemy();

	void moveEnemy(float x, float z, World w);

	bool enemyHitTest(World w);
};

