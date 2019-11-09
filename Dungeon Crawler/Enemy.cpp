#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(CModelMd2* model, float x, float z)
{
	enemy = model;
	enemy->SetPosition(x, z);
	enemy->SetHealth(2);

	isAlive = true;
	attackCooldown = false;

	cooldownStart = time(NULL);
}

Enemy::~Enemy(void)
{
}

void Enemy::updateEnemy(Uint32 time)
{
	if (isAlive)
	{
		enemy->Update(time);
	}
	else
	{
		enemy->SetPosition(2000000000,200000000000);
	}
}

void Enemy::render3DEnemy()
{
	if (isAlive)
	{
		if (!isMoving)
		{
			enemy->AnimateModel(0, 29, 0.5);
		}
		else
		{
			enemy->AnimateModel(30, 74, 0.5);
		}
	}
}

void Enemy::moveEnemy(float x, float z, World w)
{
	float dx = enemy->GetX() - x;
	float dz = enemy->GetZ() - z;
	bool moveValue = false;

	if(sqrt(pow(dx,2)+pow((dz),2)) < 3000 && sqrt(pow(dx,2)+pow((dz),2)) > 100)
	{
		enemy->SetDirectionRotationTowardsXZ(x,z);
		moveValue = true;
		if (!enemyHitTest(w))
		{
			enemy->Move(10);
		}
	}

	isMoving = moveValue;
}

bool Enemy::enemyHitTest(World w)
{
	bool returnValue = false;
	for each (Tile *pTile in w.tiles)
	{
		for each (CModel *pWall in pTile->getWalls())
		{
			if (enemy->HitTestMove(pWall))
			{
				returnValue = true;
			}
		}
	}
	for each (Chest *pChest in w.chests)
	{
		if (enemy->HitTestMove(pChest->chestBase))
		{
			returnValue = true;
		}
	}
	return returnValue;
}