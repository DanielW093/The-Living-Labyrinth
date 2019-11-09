#pragma once
#include "Game.h"
#include "World.h"
#include <vector>

class Minimap
{
private:
	bool seen;
	
	CTexture pTex;
	CTexture mapStraightTex;
	CTexture mapCornerTex;
	CTexture mapCrossTex;
	CTexture mapEndTex;
	CTexture mapTTex;

public:
	Minimap(void);
	~Minimap(void);

	vector<CSprite*> sprites;

	void initialiseMinimap();
	void generateMap(World world,float pX,float pZ);

	void update(Uint32 t,bool keyDown, float pX,float pZ);
	void render2Dminimap();

};

