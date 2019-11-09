#pragma once

#include "GTEC.h"

#include "Texture.h"
#include "Font.h"
#include "Sprite.h"
#include "SpriteList.h"
#include "SpriteRectangle.h"
#include "SpriteTriangle.h"
#include "SpritePoint.h"
#include "Sound.h"
#include "Floor.h"
#include "Model.h"
#include "ModelPoint.h"
#include "Light.h"
#include "MD2model.h"
#include "ModelList.h"


using namespace std;


class CGame
{
public:
	CGame(void);
	virtual ~CGame(void);

	// ------------  Game Modes -------------
	enum GAMEMODE { MODE_MENU, MODE_RUNNING, MODE_PAUSED, MODE_GAMEOVER };
    
	
protected:
	
	int Height, Width;   				// width and height of the game

	// -------------- Game State Flags --------
	GAMEMODE GameMode;				// current game mode - either MODE_MENU, MODE_RUNNING, MODE_PAUSED or MODE_GAMEOVER

	
	// Game Timing
	Uint32 GameTime;		// the time the game has been running for in ms
	Uint8* kbarray;  		// pointer to keyboard states array


public:
	// ---------  Accessor Functions ---------------
	
	int GetWidth()						{ return Width; }
	int GetHeight()						{ return Height; }
	void SetSize(int width, int height)			{ Width = width; Height = height; }
	
	
	// ----   Time Functions ------------------------
	
	void SetGameTime(Uint32 time)		{ GameTime = time; }
	Uint32 GetTime()					{ return GameTime; }
	
	

	// -----------  Game States ---------------------

	bool IsRunning()			{ return GameMode == MODE_RUNNING; }
	bool IsPaused()					{ return GameMode == MODE_PAUSED; }
	bool IsMenuMode()			{ return GameMode == MODE_MENU; }
	bool IsGameOver()			{ return GameMode == MODE_GAMEOVER; }
	
	GAMEMODE GetGameMode()			{ return GameMode; }
	void SetGameMode(GAMEMODE mode)	{ GameMode = mode; }	
	

	void StartGame() 			{ SetGameMode(MODE_RUNNING); OnStartGame(); }
	void GameOver() 			{ SetGameMode(MODE_GAMEOVER); ShowMouse(); }
	void NewGame()				{ SetGameMode(MODE_MENU); }
	void PauseGame()			{ SetGameMode(MODE_PAUSED); }
	void ResumeGame()			{ SetGameMode(MODE_RUNNING); }


        // --------------- Mouse Functions ----------

        void HideMouse()			{ SDL_ShowCursor( false); }
        void ShowMouse()			{ SDL_ShowCursor( true); }

 	// ---------------- Game Cycle Functions ------

	virtual void OnInitialize()		{ }
	virtual void OnDisplayMenu()		{ }
	virtual void OnStartGame()		{ }
	virtual void OnGameOver()		{ }
	virtual void OnTerminate()		{ }
	virtual void OnInitializeLevel() 	{ }

	
	virtual void OnUpdate()			{ }
	virtual void OnDraw()			{ }
    virtual void OnRender3D()		{ }
    
    void DrawCoordinateSystem();
	

	
	// ------- Event Dispatcher  ---------
	void DispatchEvents(SDL_Event* pEvent);

	
	// ---------------  Event Handling ------------

	// Check if a particular key is pressed
    bool IsKeyDown(SDLKey sym) { if (kbarray[sym]==1) return true; return false;}	

	// Keyboard events
	virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
	virtual void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);

	// Mouse events
	virtual void OnMouseMove(Uint16 x,Uint16 y,Sint16 relx,Sint16 rely,bool bLeft,bool bRight,bool bMiddle);
	virtual void OnLButtonDown(Uint16 x,Uint16 y);
	virtual void OnLButtonUp(Uint16 x,Uint16 y);
	virtual void OnRButtonDown(Uint16 x,Uint16 y);
	virtual void OnRButtonUp(Uint16 x,Uint16 y);
	virtual void OnMButtonDown(Uint16 x,Uint16 y);
	virtual void OnMButtonUp(Uint16 x,Uint16 y);

	// Window Events
	virtual void OnMinimize();
	virtual void OnRestore();
	virtual void OnResize(int w,int h);
	virtual void OnExpose();


	
};
