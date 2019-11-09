#include "Minimap.h"


Minimap::Minimap(void)
{
	seen = false;
}


Minimap::~Minimap(void)
{
}

void Minimap::initialiseMinimap()
{
	pTex.LoadTextureFromBitmap("textures/Hud/player.bmp");

	mapStraightTex.LoadTextureFromBitmap("textures/Hud/maptilestraight.bmp");
	mapCornerTex.LoadTextureFromBitmap("textures/Hud/maptilecorner.bmp");
	mapCrossTex.LoadTextureFromBitmap("textures/Hud/maptilecross.bmp");
	mapEndTex.LoadTextureFromBitmap("textures/Hud/maptileend.bmp");
	mapTTex.LoadTextureFromBitmap("textures/Hud/maptilet.bmp");
}

void Minimap::generateMap(World w,float pX,float pZ)
{	
	sprites.clear();
	for each(Tile* pTile in w.tiles)
	{
		float tileX = pTile->GetTileX();
		float tileZ = pTile->GetTileZ();

		float mapPosX = (tileX/100);
		float mapPosZ = (tileZ/100);

		//cout << "Map Pos X : " << mapPosX << "Map Pos Y : "<< mapPosZ << endl;
		CSprite* pSprite = new CSprite(mapPosX+512,-mapPosZ+384);

		pSprite->SetHealth(0);

		pSprite->SetRotation(pTile->GetTileRotation());

		if (pTile->getTileType() == "EndHall")
		{
			pSprite->SetTexture(mapEndTex);
			pSprite->SetRotation(-pSprite->GetRotation());
		}
		else if (pTile->getTileType() == "StraightHall")
		{
			pSprite->SetTexture(mapStraightTex);
		}
		else if (pTile->getTileType() == "CornerHall")
		{
			pSprite->SetTexture(mapCornerTex);
		}
		else if (pTile->getTileType() == "CrossHall")
		{
			pSprite->SetTexture(mapCrossTex);
		}
		else if (pTile->getTileType() == "THall")
		{
			pSprite->SetTexture(mapTTex);
		}

		//pSprite->SetColor(0,0,255,100);
		pSprite->SetSize(10,10);
		sprites.push_back(pSprite);
		//cout<< "++ to map blocks\n";
	}

	
	float pmapPosX = (pX/100);
	float pmapPosZ = (pZ/100);
	CSprite* pSprite = new CSprite(pmapPosX+512,-pmapPosZ+384);
	pSprite->SetHealth(1);
	pSprite->SetTexture(pTex);
	//pSprite->SetColor(0,0,255,100);
	pSprite->SetSize(2,2);
	sprites.push_back(pSprite);
}

void Minimap::update(Uint32 t,bool keyDown, float pX,float pZ)
{
	sprites.pop_back();
	float pmapPosX = (pX/100);
	float pmapPosZ = (pZ/100);

	for each (CSprite* pSprite in sprites)
	{
		pSprite->Update(t);
		if (pSprite->HitTest(pmapPosX+512, -pmapPosZ+384))
		{
			pSprite->SetHealth(1);
		}
	}	
	if(keyDown == true)
	{
		seen = true;
	}
	else
	{
		seen = false;
	}
	
	CSprite* pSprite = new CSprite(pmapPosX+512,-pmapPosZ+384);
	pSprite->SetHealth(1);
	pSprite->SetTexture(pTex);
	//pSprite->SetColor(0,0,255,100);
	pSprite->SetSize(2,2);
	sprites.push_back(pSprite);
}

void Minimap::render2Dminimap()
{
	if(seen == true)
	{
		for each (CSprite* pSprite in sprites)
		{
			if(pSprite->GetHealth() == 1)
			{
				pSprite->Draw();
			}
		}
	}
}