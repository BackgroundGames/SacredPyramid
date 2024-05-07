#include "Weapon.h"
#include "App.h"
#include "Textures.h"

Weapon::Weapon() : Item()
{
	name.Create("weapon");
	range = 1;
	damage = 1;
	wtype = WeaponType::NO_WEAPON;
	effect = WeaponEffect::NO_EFFECT;
	subtype = ItemType::WEAPON;
}

Weapon::Weapon(int range, int damage, WeaponType wtype, WeaponEffect effect)
{
	name.Create("weapon");
	this->range = range;
	this->damage = damage;
	this->wtype = wtype;
	this->effect = effect;

	subtype = ItemType::WEAPON;
	Awake();
}

Weapon::~Weapon()
{
}

bool Weapon::Awake()
{
	Item::Awake();

	switch (wtype)
	{
	case WeaponType::SWORD:
		texture = app->tex->Load("Assets/Textures/Sword.png");
		break;

	case WeaponType::BOOK:
		texture = app->tex->Load("Assets/Textures/Book.png");
		break;

	case WeaponType::SHIELD:
		break;

	case WeaponType::WRAPS:
		break;

	case WeaponType::DAGGER:
		texture = app->tex->Load("Assets/Textures/Knife_Inv.png");
		break;

	case WeaponType::FISTS:
		break;

	default:
		break;
	}

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
