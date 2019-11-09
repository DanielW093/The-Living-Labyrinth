#include "Player.h"


Player::Player()
{
	isMoving = false;
	isAttacking = false;
	attackCool = false;
	isAlive = true;
}

Player::~Player(void)
{
}

void Player::initialisePlayer(float xpos, float zpos, float direction)
{
	playerM.LoadModel("models/Armour1.md2");
	playerTex.LoadTextureFromBitmap("textures/playerTex.bmp");	
	playerM.SetRotation(direction);
	playerM.SetDirection(direction);
	playerM.SetScale(0.03f);
	playerM.SetY(0);
	playerM.SetX(xpos);
	playerM.SetZ(zpos);
	playerM.SetTexture(playerTex);	

	swordM.LoadModel("models/Sword.md2");
	swordM.SetRotation(direction);
	swordM.SetDirection(direction);
	swordM.SetScale(0.03f);
	swordM.SetY(0);
	swordM.SetX(xpos);
	swordM.SetZ(zpos);
}

bool Player::hitTestPlayer(World w)
{
	bool returnValue = false;
	for each (Tile *pTile in w.tiles)
	{
		for each (CModel *pWall in pTile->getWalls())
		{
			if (playerM.HitTestMove(pWall))
			{
				returnValue = true;
			}
		}
	}
	for each (Chest *pChest in w.chests)
	{
		if (playerM.HitTestMove(pChest->chestBase))
		{
			returnValue = true;
		}
	}
	return returnValue;
}

void Player::updatePlayer(Uint32 time)
{
	playerM.Update(time);
}

void Player::IdlePlayer(bool animate)
{
	if (animate)
	{
		playerM.AnimateModel(105, 150, 0.4);
		swordM.AnimateModel(105, 150, 0.4);
	}
	else
	{
		playerM.AnimateModel(105, 105, 0.4);
		swordM.AnimateModel(105, 105, 0.4);
	}
}

void Player::RunningPlayer()
{
	playerM.AnimateModel(0, 71, 4);
	swordM.AnimateModel(0,71, 4);
}

void Player::AttackingPlayer()
{
	playerM.AnimateModel(75, 103, 1);
	swordM.AnimateModel(75, 103, 1);
}

void Player::DyingPlayer()
{
	playerM.AnimateModel(150, 245, 0.5);
	swordM.AnimateModel(150, 245, 0.5);

	if (playerM.GetFrame() >= 244)
	{
		playerM.SetFrame(244);
		swordM.SetFrame(244);
		isAlive = false;
	}
}

void Player::render2DPlayer()
{

}

void Player::setFacingDirection(float direction)
{
	playerM.SetRotation(direction);
	playerM.SetDirection(direction);

	swordM.SetRotation(direction);
	swordM.SetDirection(direction);
}

void Player::move(int amount)
{
	playerM.Move(amount);
	swordM.Move(amount);
}

void Player::rotate(float r)
{
	setFacingDirection(getRotation() + r);
}

void Player::addGold(int value)
{
	gold += value;
}

void Player::addPotions(int value)
{
	potions += value;
}

void Player::modifyHealth(int value)
{ 
	health += value;
}

void Player::setPosition(float x, float z)
{
	playerM.SetPosition(x, z);
	swordM.SetPosition(x, z);
}

void Player::setRotation(float rotation)
{
	playerM.SetRotation(rotation);
	swordM.SetRotation(rotation);
}

void Player::setDirection(float direction)
{
	playerM.SetDirection(direction);
	swordM.SetDirection(direction);
}

void Player::setGold(int value)
{
	gold = value;
}

void Player::setPotions(int value)
{
	potions = value;
}

void Player::setHealth(int value)
{
	health = value;
}

float Player::getYPosition()
{
	return playerM.GetY();
}

float Player::getXPosition()
{
	return playerM.GetX();
}

float Player::getZPosition()
{
	return playerM.GetZ();
}

float Player::getRotation()
{
	return playerM.GetRotation();
}

float Player::getDirection()
{
	return playerM.GetDirection();
};

int Player::getGold()
{
	return gold;
}

int Player::getPotions()
{
	return potions;
}

int Player::getHealth()
{
	return health;
}