#include "Sprite.h"

#include "GameApp.h"
#include "MyGame.h"


int main(int argc, char* argv[])
{
	CGameApp app;
	CMyGame game;
	
	app.OpenWindow(1024, 768, "The Living Labyrinth");
    //app.OpenFullScreen();

	app.Run(&game);
	return(0);
}
