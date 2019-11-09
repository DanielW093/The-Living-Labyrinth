
#include "SpriteList.h"

bool CSpriteList::add( CSprite* sprite)
{
  if (number_of_items < maxSprites)
  {
    spritelist[number_of_items] = sprite;
    number_of_items++;
    return true;
  }
  return false;
}

	
void CSpriteList::erase( int n)
{
  if (n < number_of_items)
  {
    delete spritelist[n];
    spritelist[n]=spritelist[ number_of_items-1];
    number_of_items--;
  }
}

void CSpriteList::erase_marked()
{
  int n;
  for (n=0; n < number_of_items; n++)
		if (spritelist[n]->IsMarked())
		{
		   delete spritelist[n];
		   spritelist[n]=spritelist[ number_of_items-1]; 
		   number_of_items--; n--;
		}
}

void CSpriteList::clear()
{
  int n;
  for (n=0; n < number_of_items; n++) delete spritelist[n]; 
  number_of_items = 0;
}

void CSpriteList::Draw()
{
  int n;
  for (n=0; n < number_of_items; n++) spritelist[n]->Draw();
}

void CSpriteList::Update( Uint32 GameTime)
{
  int n;
  for (n=0; n < number_of_items; n++) spritelist[n]->Update( GameTime);
}
