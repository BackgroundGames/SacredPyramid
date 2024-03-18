#include "Weapon.h"

Weapon::Weapon() : Item()
{
	name.Create("weapon");
	subtype = ItemType::WEAPON;
}

Weapon::~Weapon()
{
}

bool Weapon::Awake()
{
	Item::Awake();
	return true;
}

bool Weapon::Start()
{
	Item::Start();
	return true;
}

bool Weapon::Update(float dt)
{
	Item::Update(dt);
	return true;
}

bool Weapon::CleanUp()
{
	return true;
}
