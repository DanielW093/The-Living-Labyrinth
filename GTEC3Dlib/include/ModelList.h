
#include "Model.h"

#define maxModels 500

// ---- Implementation of a simple Vector list for models ------
class CModelList
{
public:
	CModelList() { number_of_items = 0; }
	~CModelList() 
	{ 
	  clear();
	}

private:
	
CModel* modellist[ maxModels];
int number_of_items;

public:

// returns the number of model in the list
int size() { return number_of_items; }

// adds a new model at the back of the list	
bool add( CModel* sp);

// adds a new model at the back of the list (to be compatible with vectors)
void push_back( CModel* sp) 	{ add( sp); }

// returns the nth model in the list
CModel* operator[] (int n)	   { return modellist[n]; }

// removes and deletes the nth model from the list
void erase(int n);

// removes and deletes all marked model from the list
void erase_marked();

// removes and deletes all models from the list
void clear();

// calls the draw function of each model in the list
void Draw(bool box=false);

// calls the animation function of each model in the list
void AnimateModel(int startFrame, int endFrame, float percent, bool box=false);

// calls the update function of each model in the list
void Update( Uint32 GameTime);
		
};

