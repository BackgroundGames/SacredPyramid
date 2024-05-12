#include "Weapon.h"
#include "App.h"
#include "Textures.h"

Weapon::Weapon() : Item()
{
	name.Create("weapon");
	subtype = ItemType::WEAPON;

	range = 1;
	damage = 1;
	wtype = "no_weapon";
	effect = "no_effect";

	Awake();
}

Weapon::Weapon(int range, int damage, string wtype, string effect)
{
	name.Create("weapon");
	subtype = ItemType::WEAPON;

	this->range = range;
	this->damage = damage;
	this->wtype = wtype;
	this->effect = effect;

	Awake();
}

Weapon::~Weapon()
{
}

bool Weapon::Awake()
{
	Item::Awake();

	if (wtype == "sword")
		texture = app->tex->Load("Assets/Textures/Sword.png");

	if (wtype == "book")
		texture = app->tex->Load("Assets/Textures/Book.png");
	
	if (wtype == "dagger")
		texture = app->tex->Load("Assets/Textures/Knife_Inv.png");
		
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
