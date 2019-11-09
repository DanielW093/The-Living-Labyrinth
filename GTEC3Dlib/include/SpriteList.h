
#include "Sprite.h"

#define maxSprites 500

// ---- Implementation of a simple Vector list for sprites ------
class CSpriteList
{
public:
	CSpriteList() { number_of_items = 0; }
	~CSpriteList() { clear(); }

private:
	
CSprite* spritelist[ maxSprites];
int number_of_items;

public:

// returns the number of sprites in the list
int size() { return number_of_items; }

// adds a new sprite at the back of the list	
bool add( CSprite* sp);

// adds a new sprite at the back of the list (to be compatible with vectors)
void push_back( CSprite* sp) 	{ add( sp); }

// returns the nth sprite in the list
CSprite* operator[] (int n)	   { return spritelist[n]; }


// removes and deletes the nth sprite from the list
void erase(int n);

// removes and deletes all marked sprites from the list
void erase_marked();

// removes and deletes all sprites from the list
void clear();

// calls the draw function of each sprite in the list
void Draw();

// calls the update function of each sprite in the list
void Update( Uint32 GameTime);
		
};

