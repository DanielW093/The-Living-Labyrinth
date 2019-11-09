#include "WorldProvider.h"


WorldProvider::WorldProvider(void)
{
	//world = w;
	srand(time(NULL));
	curX = 0;
	curZ = 0;

}


WorldProvider::~WorldProvider(void)
{
}

void WorldProvider::initialiseWorld()
{
	//Load floor
	floor.LoadModel("models/2DPlane.obj");
	floor.SetScale(100.0f);
	floorTex.LoadTextureFromBitmap("textures/ground.bmp");
	floor.SetTexture(floorTex);

	wallTex.LoadTextureFromBitmap("textures/wallTex.bmp");
	straightWallTex.LoadTextureFromBitmap("textures/straightWallTex.bmp");

	straightWall.LoadModel("models/StraightWall.obj");
	straightWall.SetScale(100.0f);
	straightWall.SetTexture(wallTex);

	corner1.LoadModel("models/CornerWallOutside1.obj");
	corner1.SetScale(100.0f);
	corner1.SetTexture(wallTex);

	corner2.LoadModel("models/CornerWallOutside2.obj");
	corner2.SetScale(100.0f);
	corner2.SetTexture(wallTex);

	pillar.LoadModel("models/CornerWallInside.obj");
	pillar.SetScale(100.0f);
	pillar.SetTexture(wallTex);

	chestTex.LoadTextureFromBitmap("textures/chestTex.bmp");

	chest.LoadModel("models/chestBase.md2");
	chest.SetScale(100.0f);
	chest.SetTexture(chestTex);

	chestLid.LoadModel("models/chestTop.md2");
	chestLid.SetScale(0.1f);
	chestLid.SetTexture(chestTex);

	enemy.LoadModel("models/ooze.md2"); enemy.SetScale(0.02f);
	enemyTex.LoadTextureFromBitmap("textures/Enemy/slimeText.bmp");
	enemy.SetTexture(enemyTex);
}

void WorldProvider::generateWorld(int size, int level)
{
	world.tiles.clear();
	world.chests.clear();
	world.enemies.clear();

	curX = 0;
	curZ = 0;

	if (level == 1)
	{
		world.tiles.push_back(createTile(21, 0,0,90));
		world.tiles.push_back(createTile(21, 1000,0,180));
		world.tiles.push_back(createTile(21, 1000, -1000, 270));
		world.tiles.push_back(createTile(21, 0, -1000, 0));
	}

	if (level == 2)
	{
		bool endsCapped = false;
		bool endGen = false;
		int num = 0;      
		int rotation = 0;
		//Create start position

		cout << "Starting World Gen\n";
		world.tiles.push_back(createTile(20, curX,curZ,rotation)); //Creates the starting tile
		world.tiles[0]->openSides[0] = 1;

		while(endGen == false && world.tiles.size() < size + 1)  //World Generation Loop
		{
			endGen = true;
			Tile* tile;
			int b = 0;
			int curSize = world.tiles.size();
			//-------------------------------THIS LOOP BELOW WAS ORIGINALLY A FOR EACH, BUT WE KEPT GETTING AN ERROR--------------------
			//-----------------------------------IT KEPT SAYING VECTOR SUBSCRIPT OUT OF RANGE, AND WE DON'T KNOW WHY!-------------------
			do
			{
				curX = world.tiles[b]->GetTileX();
				curZ = world.tiles[b]->GetTileZ();
				for(int i = 0; i <4; i++)
				{
					//cout << "CHECK OPEN SIDE " << i << endl;
					if(world.tiles[b]->openSides[i] == 1)
					{
						endGen = false;
						if (world.tiles[b]->getTileType() == "CrossHall")
						{
							//Cross halls always spawn straights
							num = 1;   
						}
						else if (world.tiles[b]->getTileType() == "CornerHall")
						{
							//Corner Halls always spawn straights
							num = 1;
						}
						else if (world.tiles[b]->getTileType() == "THall")
						{
							//T Halls always spawn straights
							num = 1;
						}
						else
						{
							//Otherwise, any type of hall may be created (slightly more likely to be straights though)
							num = rand()%7;				
						}
						rotation = calculateRotation(num, i);
						switch(i)
						{
							case 0:
								//Up
								if (shouldCreate(0, -1000))
								{
									tile =(createTile(num,curX,curZ-1000,rotation));
									tile->openSides[2] = 0; //This tile has been placed upwards of the current tile. This closes the bottom.
								}
								break;
							case 1:
								//Right
								if (shouldCreate(1000, 0))
								{
									tile =(createTile(num,curX+1000,curZ,rotation));
									tile->openSides[3] = 0;
								}
								break;
							case 2:
								//Down
								if (shouldCreate(0, 1000))
								{
									tile =(createTile(num,curX,curZ+1000,rotation));
									tile->openSides[0] = 0;
								}
								break;
							case 3:
								//Left
								if (shouldCreate(-1000, 0))
								{
									tile =(createTile(num,curX-1000,curZ,rotation));
									tile->openSides[1] = 0;
								}
								break;
						}
						world.tiles[b]->openSides[i] = 0;
						world.tiles.push_back(tile);
						//cout << "Side complete\n";
					}				
				}
			b++;		
			} while (b < curSize);
			//cout <<"World Gen Loop Completed\n";
		}
	
		//This caps off tunnels that are open ended
		if (endGen == false && world.tiles.size() >= size)
		{
			int finishedLevel = world.tiles.size();
			int counter = 0;
			Tile* capTile;
			cout << "Capping Tunnels" << endl;
			while (counter < finishedLevel)
			{
				for(int i = 0; i <4; i++)
				{
					//cout << "CHECK OPEN SIDE " << i << endl;
					if(world.tiles[counter]->openSides[i] == 1)
					{
						if(world.tiles[counter] != NULL) // Null check "Bad coding" attempt to patch a hole on a submarine 
						{
							curX = world.tiles[counter]->GetTileX();
							curZ = world.tiles[counter]->GetTileZ();
						}
						rotation = calculateRotation(20, i);
						switch(i)
						{
							case 0:
								//Up
								if (shouldCreate(0, -1000))
								{
									capTile =(createTile(20,curX,curZ-1000,rotation));
									capTile->openSides[2] = 0;
								}
								break;
							case 1:
								//Right
								if (shouldCreate(1000, 0))
								{
									capTile =(createTile(20,curX+1000,curZ,rotation));
									capTile->openSides[3] = 0;
								}
								break;
							case 2:
								//Down
								if (shouldCreate(0, 1000))
								{
									capTile =(createTile(20,curX,curZ+1000,rotation));
									capTile->openSides[0] = 0;
								}
								break;
							case 3:
								//Left
								if (shouldCreate(-1000, 0))
								{
									capTile =(createTile(20,curX-1000,curZ,rotation));
									capTile->openSides[1] = 0;
								}
								break;
						}
						world.tiles[counter]->openSides[i] = 0;
						world.tiles.push_back(capTile);
					}	
				}
				counter++;
			} 
			endsCapped = true;
		}

		if (endsCapped == true)
		{
			cout << "Placing Chests" << endl;
			int finishedLevel = world.tiles.size();
			int chestCheck = 0;
			while (chestCheck < finishedLevel)
			{
				if (world.tiles[chestCheck]->getTileType() != "CrossHall")
				{
					createChest(world.tiles[chestCheck]->GetTileX(), world.tiles[chestCheck]->GetTileZ(), world.tiles[chestCheck]->getTileType() ,world.tiles[chestCheck]->GetTileRotation());
				}
				chestCheck++;
			}
			cout << "Amount of chests: " << world.chests.size() << endl;

			cout << "Placing Enemies" << endl;
			int enemyCheck = 0;
			while (enemyCheck < finishedLevel)
			{
				int chance = rand() % 10;

				if (world.tiles[enemyCheck]->getTileType() != "CrossHall" && chance == 0 && (world.tiles[enemyCheck]->GetTileX() != 0 && world.tiles[enemyCheck]->GetTileZ() != 0))
				{
					world.enemies.push_back(createEnemy(enemy.clone(), world.tiles[enemyCheck]->GetTileX(), world.tiles[enemyCheck]->GetTileZ()));
				}
				enemyCheck++;
			}
			cout << "Amount of enemies: " << world.enemies.size() << endl;
			
		}
	}
}

Tile* WorldProvider::createTile(int a, float x,float z,int rotation)
{
	Tile* tile = new Tile(x, z, rotation,floor.clone(),straightWall.clone(),corner1.clone(),corner2.clone(),pillar.clone());

	if (a >= 0 && a < 4)
	{
		tile->createStraightHall();
	}
	else if (a == 4)
	{
		tile->createCornerHall();
	}
	else if (a == 5)
	{
		tile->createCrossHall();
	}
	else if (a == 6)
	{
		tile->createTHall();
	}
	else if (a == 20)
	{
		tile->createEndHall();
	}
	else if (a == 21)
	{
		tile->createSpawnCorner();
	}
	
	return tile;
}

Enemy* WorldProvider::createEnemy(CModelMd2* model, float x, float z)
{
		Enemy* enemy = new Enemy(model->clone(), x, z);
		return enemy;
}

int WorldProvider::calculateRotation(int hallType, int side)
{
	int rotation;
	int direction;

	//Check what type of hall is being created, then inside what side it's being spawned on
	if (hallType >= 0 && hallType < 4)
	{
		//Halltype is straight
		if (side == 0 || side == 2)
		{
			rotation = 0;
		}
		else if (side == 1 || side == 3)
		{
			rotation = 90;
		}		
	}
	else if (hallType == 4)
	{
		direction = rand()%2;
		//Halltype is corner
		if (side == 0) //Up
		{
			if (direction == 0)
			{
				rotation = 0;
			}
			else if (direction == 1)
			{
				rotation = 270;
			}
		}
		else if (side == 1) //Right
		{
			if (direction == 0)
			{
				rotation = 180;
			}
			else if (direction == 1)
			{
				rotation = 270;
			}
		}
		else if (side == 2) //Down
		{
			if (direction == 0)
			{
				rotation = 90;
			}
			else if (direction == 1)
			{
				rotation = 180;
			}
		}
		else if (side == 3) //Left
		{
			if (direction == 0)
			{
				rotation = 0;
			}
			else if (direction == 1)
			{
				rotation = 90;
			}
		}
	}
	else if (hallType == 5)
	{
		//Halltype is cross
		rotation = 0;
	}
	else if (hallType == 6)
	{
		direction = rand()%3;
		//Halltype is corner
		if (side == 0) //Up
		{
			if (direction == 0)
			{
				rotation = 0;
			}
			else if (direction == 1)
			{
				rotation = 90;
			}
			else if (direction == 2)
			{
				rotation = 270;
			}
		}
		else if (side == 1) //Right
		{
			if (direction == 0)
			{
				rotation = 0;
			}
			else if (direction == 1)
			{
				rotation = 180;
			}
			else if (direction == 2)
			{
				rotation = 270;
			}
		}
		else if (side == 2) //Down
		{
			if (direction == 0)
			{
				rotation = 90;
			}
			else if (direction == 1)
			{
				rotation = 180;
			}
			else if (direction == 2)
			{
				rotation = 270;
			}
		}
		else if (side == 3) //Left
		{
			if (direction == 0)
			{
				rotation = 0;
			}
			else if (direction == 1)
			{
				rotation = 90;
			}
			else if (direction == 2)
			{
				rotation = 180;
			}
		}
	}
	else if (hallType == 20)
	{
		//Halltype is end
		if (side == 0) //Up
		{
			rotation = 180;
		}
		else if (side == 1) //Right
		{
			rotation = 270;
		}
		else if (side == 2) //Down
		{
			rotation = 0;
		}
		else if (side == 3) //Left
		{
			rotation = 90;
		}
	}
	return rotation;
}


World WorldProvider::getWorld()
{
	return world;
}

bool WorldProvider::shouldCreate(int x,int z)
{
	bool sC = true;

	for each(Tile* pTile2 in world.tiles)
	{
		if (pTile2->GetTileX() == curX+x && pTile2->GetTileZ() ==curZ+z)
		{
			sC = false;	
		}
	}			
	return sC;
}

void WorldProvider::createChest(float x, float z, string TileType, int tileRotation)
{
	int chance = rand() % 5;
	
	if (chance == 0)
	{
		float chestX;
		float chestZ;
		float lidX;
		float lidZ;
		float chestRotation;		

		chestRotation = tileRotation;

		if (TileType == "EndHall")
		{
			switch (tileRotation)
			{
				case 0:
					chestX = x;
					chestZ = z + 300;
					lidX = x;
					lidZ = z + 281;
					break;
				case 90:
					chestRotation = -tileRotation;
					chestX = x - 300;
					chestZ = z;
					lidX = x - 281;
					lidZ = z;
					break;
				case 180:
					chestX = x;
					chestZ = z - 300;
					lidX = x;
					lidZ = z - 281;
					break;
				case 270:
					chestRotation = -tileRotation;
					chestX = x + 300;
					chestZ = z;
					lidX = x + 281;
					lidZ = z;
					break;
			}
		}
		else if (TileType == "StraightHall")
		{
			int r = rand() % 2;
			switch (tileRotation)
			{
				case 0:
					if (r == 0)
					{
						chestRotation = tileRotation+90;
						chestX = x + 300;
						chestZ = z;
						lidX = x + 281;
						lidZ = z;
					}
					else
					{
						chestRotation = tileRotation-90;
						chestX = x - 300;
						chestZ = z;
						lidX = x - 281;
						lidZ = z;
					}
					break;
				case 90:
					if (r == 0)
					{
						chestRotation = tileRotation-90;
						chestX = x;
						chestZ = z + 300;
						lidX = x;
						lidZ = z + 281;
					}
					else
					{
						chestRotation = tileRotation+90;
						chestX = x;
						chestZ = z - 300;
						lidX = x;
						lidZ = z - 281;
					}
					break;
			}
		}
		else if (TileType == "CornerHall")
		{
			switch (tileRotation)
			{
				case 0:
					chestRotation = -135;
					chestX = x - 300;
					chestZ = z - 300;
					lidX = x - 281;
					lidZ = z - 281;
					break;
				case 90:
					chestRotation = -45;
					chestX = x - 300;
					chestZ = z + 300;
					lidX = x - 281;
					lidZ = z + 281;
					break;
				case 180:
					chestRotation = 45;
					chestX = x + 300;
					chestZ = z + 300;
					lidX = x + 281;
					lidZ = z + 281;
					break;
				case 270:
					chestRotation = 135;
					chestX = x + 300;
					chestZ = z - 300;
					lidX = x + 281;
					lidZ = z - 281;
					break;
			}
		}
		else if (TileType == "THall")
		{
			switch (tileRotation)
			{
				case 0:
					chestRotation = 180;
					chestX = x;
					chestZ = z - 300;
					lidX = x;
					lidZ = z - 281;
					break;
				case 90:
					chestRotation = -90;
					chestX = x-300;
					chestZ = z;
					lidX = x - 281;
					lidZ = z;
					break;
				case 180:
					chestRotation = 0;
					chestX = x;
					chestZ = z + 300;
					lidX = x;
					lidZ = z + 281;
					break;
				case 270:
					chestRotation = 90;
					chestX = x + 300;
					chestZ = z;
					lidX = x + 281;
					lidZ = z;
					break;
			}
		}
		
		Chest* pChest = new Chest(chestX, chestZ, lidX, lidZ, chestRotation, chest.clone(), chestLid.clone());

		world.chests.push_back(pChest);		
	}
}