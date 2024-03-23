#include "Armor.h"

Armor::Armor() : Item()
{
	name.Create("armor");
	subtype = ItemType::ARMOR;
}

Armor::~Armor() 
{

}

bool Armor::Awake()
{
	Item::Awake();
	return true;
}

bool Armor::Start()
{
	Item::Start();
	return true;
}

bool Armor::Update(float dt)
{
	Item::Update(dt);
	return true;
}

bool Armor::CleanUp()
{
	return true;
}