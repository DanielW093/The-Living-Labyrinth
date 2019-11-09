#pragma once
#include "Player.h"
#include "World.h"
#include "WorldProvider.h"
#include "Minimap.h"
class CMyGame : public CGame
{
public:
	CMyGame();
	~CMyGame();

    // ----  Declare your game variables and objects here -------------


	// Variables
	int idleTimer;
	int score;
	float rotation;
	float tilt;

	int level;
	int merchantSound;
    int attackSound;
	bool deathSoundPlayed;

	bool displayHelp;

	//Sounds
	CSound merchant1;
	CSound merchant2;
	CSound merchant3;

	CSound deathSound;

	CSound intro;

	CSound drink;
	CSound pain;
	CSound attack1;
	CSound attack2;
	CSound attack3;

	//Sprites
	CSprite moneyHud;
	CSprite potionHud;
	CSprite healthBarHud;
	CSprite healthHud;
	CSprite startButton;

    // Models and Model Lists
	Player player;
	CModel marketTable;
	CModel groundCircle;
	CModel ladder;
	//Enemy enemy;

	// Textures
	CTexture mainMenuTex;
	CTexture moneyTex;
	CTexture potionTex;
	CTexture healthBarTex;
	CTexture healthTex;
	CTexture woodTex;
	CTexture black;
	CTexture buttonUp;
	CTexture buttonDown;

	CTexture sirCzes;

	// Font
	CFont font;

	//World Stuff
	World world;
	WorldProvider wProv;
	Minimap map;
   // ---------------------Event Handling --------------------------

	// Game Loop Funtions
	virtual void OnUpdate();
	virtual void OnDraw();
	virtual void OnRender3D();

	// Game Life Cycle
	virtual void OnInitialize();
	virtual void OnDisplayMenu();
	virtual void OnInitializeLevel_1();
	virtual void OnInitializeLevel_2();
	virtual void OnInitializeLevel_99();
	virtual void OnStartGame();
	virtual void OnGameOver();
	virtual void OnTerminate();

	// Keyboard Event Handlers
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse Events Handlers
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);

};
