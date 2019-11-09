#pragma once

#include <Game.h>

class Chest
{
private:
	float chestXPos;
	float chestZPos;	
public:
	Chest(float cx, float cz, float lx, float lz, float rotation, CModelMd2* baseModel, CModelMd2* lidModel);
	~Chest(void);

	bool isOpen;

	CModelMd2* chestBase;
	CModelMd2* chestLid;

	void setX(float x) { chestXPos = x; }
	void setZ(float Z) { chestZPos = Z; }
	void setOpen(bool val) { isOpen = val; }

	float getX() { return chestXPos; }
	float getZ() { return chestZPos; }
	bool getOpen() { return isOpen; }

	void updateChest(Uint32 time);
	void drawChest();
	void animateChest();

	void openChest();
};

