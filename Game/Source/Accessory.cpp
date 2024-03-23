#include "Accessory.h"

Accessory::Accessory() : Item()
{
	name.Create("Accessory");
	subtype = ItemType::ACCESSORY;
}

Accessory::~Accessory()
{

}

bool Accessory::Awake()
{
	Item::Awake();
	return true;
}

bool Accessory::Start()
{
	Item::Start();
	return true;
}

bool Accessory::Update(float dt)
{
	Item::Update(dt);
	return true;
}

bool Accessory::CleanUp()
{
	return true;
}
