#pragma once
#include <Game.h>
#include "World.h"

class Player
{
private:
	CModelMd2 playerM;
	CModelMd2 swordM;
	CTexture playerTex;

	int gold;
	int potions;
	int health;

public:
	Player();
	~Player(void);

	bool isMoving;
	bool isAttacking;
	bool attackCool;
	bool isAlive;

	void initialisePlayer(float xpos, float zpos, float direction);

	bool hitTestPlayer(World w);

	void updatePlayer(Uint32 t);
	void IdlePlayer(bool animate);
	void RunningPlayer();
	void AttackingPlayer();
	void DyingPlayer();
	void render2DPlayer();

	//For moving the player
	void move(int amount);

	//For directing the player
	void setFacingDirection(float direction);
	void rotate(float r);

	//For manipulating player values
	void addGold(int value);
	void addPotions(int value);
	void modifyHealth(int value);

	//General Setters
	void setPosition(float x, float z);
	void setRotation(float rotation);
	void setDirection(float direction);
	void setGold(int value);
	void setPotions(int value);
	void setHealth(int value);
	void setFrame(int value) { playerM.SetFrame(value); }
	//General Getters
	float getYPosition();
	float getXPosition();
	float getZPosition();
	float getRotation();
	float getDirection();
	int getGold();
	int getPotions();
	int getHealth();
	int getFrame() { return playerM.GetFrame(); }
};

