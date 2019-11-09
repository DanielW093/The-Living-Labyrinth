#include "GameApp.h"


// ----- Constructor & Destructor --------

CGameApp::CGameApp()
{
	pGame = NULL;
	pSurface = NULL;
	Height=0; Width=0;
	FPS=30; GameTime=0;
    Color.Set( CColor::Black()); // default color
	
	/* new seed for random number generator */
	srand((unsigned)time(NULL));
}

CGameApp::~CGameApp() { }


// ------ Game Time -----------

Uint32 CGameApp::GetSystemTime()
{
	// returns the number of milliseconds since the application was started
    return SDL_GetTicks();

}

// ----  Application Management ----------

bool CGameApp::OpenWindow(int width, int height, string title)
{
	/* Flags to pass to SDL_SetVideoMode */
    int videoFlags;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	Width=width; Height=height;

	/* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */	
 

    /* Create a SDL surface */
    pSurface = SDL_SetVideoMode( width, height, 16, videoFlags );

    /* Verify if there is a surface */
    if ( !pSurface ) { cout << " Error: Video not initialized" << endl; return false; }
		
	// call SQL quit when closing the game	
	atexit(SDL_Quit);
	
	SDL_WM_SetCaption( title.c_str(), title.c_str());

	InitializeGL(); SetProjection2D();

	InitializeAudio();

	cout << "Initializing engine v1.01 ..ok" << endl;

	return true;
}

bool CGameApp::OpenFullScreen(int width, int height)
{
	/* Flags to pass to SDL_SetVideoMode */
    int videoFlags;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	Width=width; Height=height;

	/* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_FULLSCREEN;      /* Enable full screen */
 
    /* Set screen size */
       
    /* Create a SDL surface */
    pSurface = SDL_SetVideoMode( width, height, 16, videoFlags );

    /* Verify if there is a surface */
    if ( !pSurface ) { cout << " Error: Video not initialized" << endl; return false; }
		
	// call SQL quit when closing the game	
	atexit(SDL_Quit);

	InitializeGL(); SetProjection2D();

	InitializeAudio();

	cout << "Initializing engine v1.01 ..ok" << endl;
	
	
	return true;
}

bool CGameApp::OpenFullScreen()
{
	/* Flags to pass to SDL_SetVideoMode */
    int videoFlags;

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		return false;

	/* the flags to pass to SDL_SetVideoMode */
    videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
    videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
    videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
    videoFlags |= SDL_FULLSCREEN;      /* Enable full screen */
 
    /* Set screen size */
    
    
    const SDL_VideoInfo* info = SDL_GetVideoInfo();
    Width = info->current_w;
    Height = info->current_h;
    
    cout << "Screen: " << Width << "x" << Height << endl;
	
	/* Create a SDL surface */
    pSurface = SDL_SetVideoMode( Width, Height, 16, videoFlags );

    /* Verify if there is a surface */
    if ( !pSurface ) { cout << " Error: Video not initialized" << endl; return false; }
		
	// call SQL quit when closing the game	
	atexit(SDL_Quit);

	InitializeGL(); SetProjection2D();

	InitializeAudio();

	cout << "Initializing engine v1.01 ..ok" << endl;
	
	
	return true;
	
	
}


void CGameApp::InitializeGL()
{
    glClearColor( Color.R, Color.G, Color.B, Color.A);

	glLineWidth( 2);
	
	/* line anti-aliasing */
	glEnable( GL_LINE_SMOOTH);
	glEnable( GL_POINT_SMOOTH);
	glShadeModel(GL_SMOOTH);

	/* enable 2D texturing */
	glEnable(GL_TEXTURE_2D);

    glEnable(GL_ALPHA_TEST);
    glAlphaFunc( GL_GREATER, 0);
    
	/* enable alpha blending */
    glEnable( GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    /* normalize normal vectors */
	glEnable(GL_NORMALIZE);
	
	/* enable color tracking */
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
}


void CGameApp::SetProjection2D()
{
	glViewport(0, 0, Width, Height);		// reset the viewport to new dimensions
	glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
	glLoadIdentity();						// reset projection matrix

	
	gluOrtho2D(0, Width, 0, Height);        // 2D projection
    glMatrixMode( GL_MODELVIEW );       	// set modelview matrix
	glLoadIdentity();	
	
	glDisable(GL_DEPTH_TEST);	
}

void CGameApp::SetProjection3D()
{
	glViewport(0, 0, Width, Height);		// reset the viewport to new dimensions
	glMatrixMode(GL_PROJECTION);			// set projection matrix current matrix
	glLoadIdentity();						// reset projection matrix
	
	// calculate perspective
	//gluPerspective(54.0f,(GLfloat)Width/(GLfloat)Height,1.0f,1000.0f);
	glFrustum(-400.0, 400.0, -300.0, 300.0, 1000.0f, 3500.0f);
	glMatrixMode(GL_MODELVIEW);				// set modelview matrix
	glLoadIdentity();						// reset modelview matrix
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}


void CGameApp::InitializeAudio()
{
   /* Audio is initialzed for 44kHz frequency 16-bit and 2 channels */
    Mix_OpenAudio( 44100, AUDIO_S16SYS, 2, 2048);
}




bool CGameApp::Run(CGame *pgame)
{
	bool exitGame = false;				// indicates whether to quit the game
	SDL_Event anEvent;					// declare an Event

	// Controlling the Time
	double period = 1000.0 / FPS;

	if (!pgame) return false;        // return false if game does not exist
	else pGame = pgame;
	
	
	pGame->SetSize(Width, Height);

		
	// --- Call In-Game Initialisation ---
	pGame->OnInitialize();

	GameTime = 0; // resetting game time at the start of the game
	
	Uint32 startTime = GetSystemTime(); // current system time in milliseconds
	
	Uint32 framecycles = 0;
	Uint32 gamecycles = 0;

	while ( !exitGame)
	{
       
        // --- Event Handling ------- 
		while ( SDL_PollEvent(&anEvent))
		{
			
			// --- Processing of exit events ---
            pGame->DispatchEvents(&anEvent);
			
            if (anEvent.type == SDL_KEYDOWN && anEvent.key.keysym.sym == SDLK_F4) { exitGame = true; break; }
			
		    if (anEvent.type == SDL_QUIT) { exitGame = true; break; }
			  
		}



		// ----- Game Over Mode -----
		if (pGame->IsGameOver())
		{
		   GameTime=0; gamecycles=0;
           pGame->SetGameTime( GameTime);
		}
       

		// ---- Game Running: Updating the game time ------
		if (pGame->IsRunning())
		{
		   gamecycles++;
           GameTime = Uint32(gamecycles*period);
		   pGame->SetGameTime( GameTime);
		   pGame->OnUpdate();
		}
		
		
		// clear screen and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
		
		
		// set 3D projection
		SetProjection3D();
		
		// -------  game world transformations -------
		// transformations applied to affect the whole game world
		// to change camera position here
		
		glTranslatef( 0, 0, -2000); // move game world into frustum
		
		//glRotatef(  55, 1, 0, 0);  // tilt game world 
		//glRotatef( 45, 0, 1, 0 );  // rotate round the y axis
		
		
		// ----- render the 3D game world ------------
		pGame->OnRender3D();  // render 3D scene and objects
		
		// set 2D projection		
		SetProjection2D();
				
		if (pGame->IsMenuMode())
		{
           pGame->OnDisplayMenu(); // draw menu screen
		   
		}
		else pGame->OnDraw(); // draw 2D screen and sprites

	      	
		Uint32 timeStamp2 = Uint32(framecycles*period + startTime); 

		while (timeStamp2 > GetSystemTime())
		{
			SDL_Delay(1);
		}
  
		// flip buffers
		SDL_GL_SwapBuffers();

		framecycles++;
	}

	pGame->OnTerminate();
	SDL_Quit();
	return true;
}

