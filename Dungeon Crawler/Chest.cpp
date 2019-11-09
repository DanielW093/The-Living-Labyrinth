#include "Chest.h"


Chest::Chest(float cx, float cz, float lx, float lz, float rotation, CModelMd2* baseModel, CModelMd2* lidModel)
{
	chestBase = baseModel;
	chestLid = lidModel;

	chestBase->SetPosition(cx, cz);
	chestBase->SetRotation(rotation);
	chestBase->SetScale(100.0f);

	chestLid->SetPosition(lx, lz);
	chestLid->SetRotation(rotation);
	chestLid->SetY(115);
	chestLid->SetScale(0.1f);

	chestXPos = cx;
	chestZPos = cz;

	isOpen = false;
}


Chest::~Chest(void)
{
}

void Chest::updateChest(Uint32 time)
{
	chestBase->Update(time);
	chestLid->Update(time);
}

void Chest::drawChest()
{
	chestBase->Draw();
	chestLid->Draw();
}

void Chest::animateChest()
{
	chestBase->Draw();

	if(chestLid->GetFrame() < 15)
	{
		chestLid->AnimateModel(0, 16, 0.5);
	}
	else
	{
		chestLid->Draw();
	}
}

void Chest::openChest()
{
	isOpen = true;

	//play sounds

}