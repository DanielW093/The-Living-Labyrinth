
#include "ModelList.h"

bool CModelList::add( CModel* model)
{
  if (number_of_items < maxModels)
  {
    modellist[number_of_items] = model;
    number_of_items++;
    return true;
  }
  return false;
}

	
void CModelList::erase( int n)
{
  if (n < number_of_items)
  {
    delete modellist[n];
    modellist[n]=modellist[ number_of_items-1];
    number_of_items--;
  }
}

void CModelList::erase_marked()
{
  int n;
  for (n=0; n < number_of_items; n++)
		if (modellist[n]->IsMarked())
		{
		   delete modellist[n];
		   modellist[n]=modellist[ number_of_items-1]; 
		   number_of_items--; n--;
		}
}

void CModelList::clear()
{
  int n;
  for (n=0; n < number_of_items; n++) delete modellist[n]; 
  number_of_items = 0;
}

void CModelList::Draw(bool box)
{
  int n;
  for (n=0; n < number_of_items; n++) modellist[n]->Draw(box);
}

void CModelList::AnimateModel(int startFrame, int endFrame, float percent, bool box)
{
  int n;
  for (n=0; n < number_of_items; n++) modellist[n]->AnimateModel(startFrame, endFrame, percent, box);
}

void CModelList::Update( Uint32 GameTime)
{
  int n;
  for (n=0; n < number_of_items; n++) modellist[n]->Update( GameTime);
}
