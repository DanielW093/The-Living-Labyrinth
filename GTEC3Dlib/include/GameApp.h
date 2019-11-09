#pragma once

#include "Game.h"
#include "Color.h"


class CGameApp
{

private:
	CGame *pGame;				// Reference to the Game
	SDL_Surface *pSurface;		// Reference to screen surface
	
	Uint16 Height,Width;    	// Width and height of the game
	Uint16 FPS;					// Frames per Second rate
	Uint32 GameTime;			// Game Time since the game was started
	
	CColor Color;				// Background colour (default is black)


public:
	
	// --- Constructor & Destructor ----------
	CGameApp();
	~CGameApp();

	// ----- Setter and Getter Functions -----
	
    int GetWidth()						{ return Width; }
	int GetHeight()						{ return Height; }
	
	int GetFPS()						{ return FPS; }
	void SetFPS(unsigned short fps)	{ FPS = fps; }
	
	void SetClearColor( const CColor& color)  { Color.Set( color); }
	
	// ------- The System Clock ------------------

	Uint32 GetSystemTime();	   // returns the number of milliseconds since the application was started
	
	// -----------    Open Window/Full Screen -----------
	bool OpenWindow(int width, int height, string title);
	bool OpenFullScreen(int width, int height);
	bool OpenFullScreen();

	// -----------   Main Game Loop ----------------------
	bool Run(CGame *pGame = NULL);
	
private:

    // -------------- Initialize OpenGL for 2D & 3D ------------------
    void InitializeGL();
    
    void SetProjection2D();
    
    void SetProjection3D();

    // -------------- Initialize Audio using SDL Mixer ------------------
	void InitializeAudio();

};
