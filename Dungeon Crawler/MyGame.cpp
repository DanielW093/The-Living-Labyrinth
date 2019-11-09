#include "MyGame.h"

using namespace std;

CMyGame::CMyGame(void) : player()
{
} 

CMyGame::~CMyGame(void) {}

// --------  game initialisation --------
void CMyGame::OnInitialize()
{
	idleTimer = time(NULL);
	cout << "Loading assets..." << endl;
	mainMenuTex.LoadTextureFromBitmap("textures/Menu/menuScreen.bmp"); mainMenuTex.SetZoom(110);
	sirCzes.LoadTextureFromBitmap("textures/Menu/sirCzes.bmp");

	// Loading graphics and sound assets
	moneyTex.LoadTextureFromBitmap("textures/Hud/Inventory/coins.bmp");
	moneyHud.SetTexture(moneyTex); moneyHud.SetSize(32, 32); moneyHud.SetPosition(20, 750);
	potionTex.LoadTextureFromBitmap("textures/Hud/Inventory/potion.bmp");
	potionHud.SetTexture(potionTex); potionHud.SetSize(32, 32); potionHud.SetPosition(20, 710);
	healthBarTex.LoadTextureFromBitmap("textures/Hud/healthBar.bmp");
	healthBarHud.SetTexture(healthBarTex); healthBarHud.SetPosition(115, 26);
	healthTex.LoadTextureFromBitmap("textures/Hud/health.bmp");
	healthHud.SetTexture(healthTex); healthHud.SetPosition(115, 26);

	buttonUp.LoadTextureFromBitmap("textures/Menu/playUp.bmp");
	buttonDown.LoadTextureFromBitmap("textures/Menu/playDown.bmp");
	startButton.SetTexture(buttonUp);
	
	marketTable.LoadModel("models/marketTable.obj"); woodTex.LoadTextureFromBitmap("textures/wood.bmp"); marketTable.SetTexture(woodTex);
	
	groundCircle.LoadModel("models/groundCircle.obj"); black.LoadTextureFromBitmap("textures/black.bmp"); groundCircle.SetTexture(black);
	
	ladder.LoadModel("models/Ladder.obj"); ladder.SetTexture(woodTex);

	//Load Sounds
	merchantSound = 1;
	merchant1.LoadAudioFile("sounds/Merchant-WithoutMyUpgrades.wav");
	merchant2.LoadAudioFile("sounds/Merchant-Potions.wav");
	merchant3.LoadAudioFile("sounds/Merchant-Birds.wav");
	deathSound.LoadAudioFile("sounds/Misc-Death.wav");
	intro.LoadAudioFile("sounds/SirCzes-Story.wav");

	drink.LoadAudioFile("sounds/Misc-Drink.wav");
	pain.LoadAudioFile("sounds/Misc-Pain.wav");
	attackSound = 1;
	attack1.LoadAudioFile("sounds/Misc-Swing.wav");
	attack2.LoadAudioFile("sounds/Misc-Swing2.wav");
	attack3.LoadAudioFile("sounds/Misc-Swing3.wav");

	player.initialisePlayer(0.0f, 0.0f, 90.0f);
	wProv.initialiseWorld();
	map.initialiseMinimap();
	font.LoadDefault(); font.SetColor(CColor::White());
	// Enable lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	//---Lighting--
	GLfloat ambientLight[] = {0.2f,0.2f,0.2f,1.0f}; //Color (0.2,0.2,0.2)
	GLfloat diffuseLight[] = {0.8,0.8,0.8,1.0};
	GLfloat specularLight[] = {1.0,1.0,1.0,1.0};

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	//Set Variables
	player.setGold(400);
	player.setPotions(0);
	player.setHealth(100);

	// game world rotation and tilt
	rotation= 0;  tilt=60;
	score=0; level = 0;

	deathSoundPlayed = false;
	displayHelp = true;
}

void CMyGame::OnInitializeLevel_99()
{
	level = -1;
	intro.Play();
}
void CMyGame::OnInitializeLevel_1()
{ 
	level = 1;
	wProv.generateWorld(0, 1);
	world.setWorld(wProv.getWorld());
	marketTable.SetPosition(500,-500); marketTable.SetScale(96.0f); marketTable.SetRotation(90);
	groundCircle.SetPosition(0, -1000); groundCircle.SetScale(100.0f); groundCircle.SetY(0.003f);
	ladder.SetPosition(-45, -1045); ladder.SetY(-380); ladder.SetScale(90.0f);
	cout << "World Size: " << world.tiles.size() << endl;
}

void CMyGame::OnInitializeLevel_2()
{
	level = 2;
	wProv.generateWorld(150, 2);
	world.setWorld(wProv.getWorld()); 
	map.generateMap(world,player.getXPosition(),player.getZPosition());
	ladder.SetPosition(0, 0); ladder.SetY(0); ladder.SetScale(90.0f);
	cout << "World Size: " << world.tiles.size() << endl;
}

// Game Logic in the OnUpdate function called every frame

void CMyGame::OnUpdate() 
{
	//Player Movement
	if (level > 0)
	{
		if (IsKeyDown(SDLK_a)) 
		{ 
			player.rotate(5);
		}
		if (IsKeyDown(SDLK_d)) 
		{ 
			player.rotate(-5);
		}

		if (IsKeyDown(SDLK_w) && player.hitTestPlayer(world) == false && player.isAttacking == false) 
		{ 
			player.move(20);
			player.isMoving = true;
		}
		else
		{
			player.isMoving = false;
		}

		//Camera Movement
		if (CMyGame::IsKeyDown(SDLK_q))
		{
			rotation+=2;
		}
		if (CMyGame::IsKeyDown(SDLK_e))
		{
			rotation-=2;
		}
	}

	// --- updating models ----

	world.updateWorld(GetTime(), world, player.getXPosition(), player.getZPosition());

	if (level == 0)
	{
		
	}

	if(level == 1)
	{
	}

	if (level == 2)
	{
		map.update(GetTime(),IsKeyDown(SDLK_m), player.getXPosition(),player.getZPosition());
	}
	player.updatePlayer(GetTime());
	
	if(player.isAlive == false)
	{
		OnGameOver();
	}
}


//-----------------  Draw 2D overlay ----------------------
void CMyGame::OnDraw()
{
	glDisable(GL_LIGHTING); //Disable lighting beforehand
    
	if (level > 0 && displayHelp == true)
	{
		font.DrawTextW(700, 745, "Movement Keys: WAS");
		font.DrawTextW(700, 720, "Attack Key: Space");
		font.DrawTextW(700, 695, "Interact: F");
		font.DrawTextW(700, 670, "Drink Potion: R");
		font.DrawTextW(700, 645, "Rotate Camera: Q,E");		
		font.DrawTextW(700, 620, "Interact with chests,");
		font.DrawTextW(700, 595, "the ladder and table.");
		font.DrawTextW(700, 570, "Press F1 to hide");
	}

	// draw GTEC 2D
	if (level == -1)
	{
		sirCzes.Draw(512, 384);
		font.DrawTextW(418, 40, "Press F to skip");
	}

	if (level > 0)
	{
		moneyHud.Draw(); font.DrawNumber(40, 735, player.getGold()); 
		potionHud.Draw(); font.DrawNumber(40, 695, player.getPotions());
		map.render2Dminimap();
		player.render2DPlayer();
		healthBarHud.Draw(); 	
		healthHud.Draw(); healthHud.SetSize((player.getHealth()*2), 20); healthHud.SetPosition(115-((200-healthHud.GetWidth())/2), 26);

		font.DrawTextW(11, 38, "Health: "); font.DrawNumber(130, 38, player.getHealth());
	}

	glEnable(GL_LIGHTING); //Re-enable lighting
}

// ----------------   Draw 3D world -------------------------
void CMyGame::OnRender3D()
{
	// ------ Global Transformation Functions -----
	//glTranslatef(0,50,0);
	glRotatef(tilt,1,0,0);			// tilt game world around x axis
    glRotatef( rotation,0,1,0);		// rotate game world around y axis
	glTranslatef(-player.getXPosition(), 0, -player.getZPosition());

	// ------- Draw your 3D Models here ----------
	
	//floor.Draw(true);
	
	ladder.Draw();

	if (level == 1)
	{
		marketTable.Draw();	groundCircle.Draw(); 
	}

	world.render3DWorld();

	if (level > 0)
	{
		for each (Enemy* pEnemy in world.enemies)
		{
			if (GetTime() >= pEnemy->cooldownStart + 600)
			{
				pEnemy->attackCooldown = false;
			}

			float dx = pEnemy->enemy->GetX() - player.getXPosition();
			float dz = pEnemy->enemy->GetZ() - player.getZPosition();
			if (sqrt(pow(dx,2)+pow((dz),2)) < 150 && !pEnemy->attackCooldown)
			{
				player.setHealth(player.getHealth() - 5);
				pEnemy->attackCooldown = true;
				pain.Play();
				pEnemy->cooldownStart = GetTime();
			}
		}

		if (player.getHealth() <= 0 && player.isAlive == true)
		{
			player.setHealth(0);			
			player.DyingPlayer();
			if (deathSoundPlayed == false)
			{
				deathSound.Play();
				deathSoundPlayed = true;
			}

			if (player.getFrame() == 244)
			{
				player.isAlive = false;
			}
		}

		if (player.getHealth() > 0)
		{
			if (player.isAttacking)
			{
				player.AttackingPlayer();
				idleTimer = time(NULL);

				for each (Enemy* pEnemy in world.enemies)
				{
					float dx = pEnemy->enemy->GetX() - player.getXPosition();
					float dz = pEnemy->enemy->GetZ() - player.getZPosition();

					if (sqrt(pow(dx,2)+pow((dz),2)) < 200 && !player.attackCool)
					{
						pEnemy->enemy->SetHealth(pEnemy->enemy->GetHealth() - 1);
						cout << "Enemy Health: " << pEnemy->enemy->GetHealth() << endl;
						player.attackCool = true;
					}
				}
				if (player.getFrame() == 102)
				{
					player.isAttacking = false;
					player.attackCool = false;
				}
			}
			if (!player.isAttacking)
			{
				if (!player.isMoving)
				{		
					if (time(NULL) >= idleTimer + 3)
					{
						player.IdlePlayer(true);
						if (player.getFrame() == 149)
						{				
							idleTimer = time(NULL);			
						}
					}
					else
					{
						player.IdlePlayer(false);	
					}
				}
				else
				{
					player.RunningPlayer();
					idleTimer = time(NULL);
				}
			}
		}
	}

	//DrawCoordinateSystem();
}


// called at the start of a new game - display menu here
void CMyGame::OnDisplayMenu()
{
	level = 0;
	mainMenuTex.Draw(512, 384);
	startButton.SetPosition(525, 200);
	startButton.Draw();
	//StartGame();	// this allows to start the game immediately	
}

// called when Game Mode entered
void CMyGame::OnStartGame()
{
     OnInitializeLevel_99();	
}


// called when Game is Over
void CMyGame::OnGameOver()
{
	level = 0;
	world.tiles.clear();
	world.chests.clear();
	world.enemies.clear();
	NewGame();
}

// one time termination code
void CMyGame::OnTerminate()
{

}

// -------    Keyboard Event Handling ------------
void CMyGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	if (sym == SDLK_p)
	{
		if (IsPaused()) ResumeGame();
		else PauseGame();
	}
	
	if (sym == SDLK_F1) 
	{
		if (level > 0)
		{
			if (displayHelp)
			{
				displayHelp = false;
			}
			else
			{
				displayHelp = true;
			}
		}
	}

	if (sym == SDLK_F2) 
	{
		//NewGame();
	}

	if (sym == SDLK_r)
	{
		if (player.getHealth() < 100 && player.getPotions() > 0)
		{
			player.setHealth(player.getHealth() + 20);
			player.addPotions(-1);
			drink.Play();
		}
		if (player.getHealth() > 100)
		{
			player.setHealth(100);
		}
	}

	if (sym == SDLK_f)
	{
		if (level == -1)
		{
			OnInitializeLevel_1();
			intro.Pause();
		}
		for each(Chest* pChest in world.chests)
		{
			double x1 = pChest->getX(); double z1 = pChest->getZ();
			double x2 = player.getXPosition(); double z2 = player.getZPosition();

			if (sqrt(pow(x1 - x2,2)+pow((z1-z2),2)) < 200 && !pChest->isOpen)
			{
				pChest->openChest();
				int moneyAmount = (rand() % 200) + 100;
				player.addGold(moneyAmount);
				int potionAmount = rand()%3;
				player.addPotions(potionAmount);
			}
		}

		float dx = ladder.GetX() - player.getXPosition();
		float dz = ladder.GetZ() - player.getZPosition();
		if (sqrt(pow(dx,2)+pow((dz),2)) < 200)
		{
			if (level == 1)
			{
				OnInitializeLevel_2();
				player.setPosition(0,-100);
			}
			else if (level == 2)
			{
				OnInitializeLevel_1();
				player.setPosition(-45, -1000);
			}
		}

		float dxm = marketTable.GetX() - player.getXPosition();
		float dzm = marketTable.GetZ() - player.getZPosition();

		if (sqrt(pow(dxm,2)+pow((dzm),2)) < 200)
		{
			if (level == 1)
			{
				if (player.getGold() >= 100)
				{
					player.addGold(-100);
					player.addPotions(1);

					merchant1.Pause();
					merchant2.Pause();
					merchant3.Pause();

					if (merchantSound == 1)
					{
						merchant1.Play();
						merchantSound++;
					}
					else if (merchantSound == 2)
					{
						merchant2.Play();
						merchantSound++;
					}
					else if (merchantSound == 3)
					{
						merchant3.Play();
						merchantSound = 1;
					}
				}
			}
		}
	}

	if (sym == SDLK_SPACE && player.isAttacking == false)
	{
		player.isAttacking = true;
		if (attackSound == 1)
		{
			attack1.Play();
			attackSound = 2;
		}
		else if (attackSound == 2)
		{
			attack2.Play();
			attackSound = 3;
		}
		else if (attackSound == 3)
		{
			attack3.Play();
			attackSound = 1;
		}
	}

	if (sym == SDLK_u)
	{
		for each (CSprite* pSprite in map.sprites)
		{
			pSprite->SetHealth(1);
		}
	}
}

void CMyGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
	
}

// -----  Mouse Events Handlers -------------

void CMyGame::OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle)
{
	// player.inv.mouseX = x;
	// player.inv.mouseY = y;
	if (x >= startButton.GetX()-(startButton.GetWidth()/2) && x <= startButton.GetX() + (startButton.GetWidth()/2) && 
		y >= startButton.GetY()-(startButton.GetHeight()/2) && y <= startButton.GetY() + (startButton.GetHeight()/2))
	{
		startButton.SetTexture(buttonDown);
	}
	else
	{
		startButton.SetTexture(buttonUp);
	}
}

void CMyGame::OnLButtonDown(Uint16 x,Uint16 y)
{    
	if (x >= startButton.GetX()-(startButton.GetWidth()/2) && x <= startButton.GetX() + (startButton.GetWidth()/2) && 
		y >= startButton.GetY()-(startButton.GetHeight()/2) && y <= startButton.GetY() + (startButton.GetHeight()/2) && level == 0)
	{
		StartGame();
	}
}

void CMyGame::OnLButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnRButtonUp(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonDown(Uint16 x,Uint16 y)
{
}

void CMyGame::OnMButtonUp(Uint16 x,Uint16 y)
{
}
