#include "Armor.h"

Armor::Armor() : Item()
{
	name.Create("Armor");
	subtype = ItemType::ARMOR;

	defense = 0;
	atype = ArmorType::NO_ARMOR;
	effect = ArmorEffect::NO_EFFECT;
}

Armor::Armor(int defense, ArmorType atype, ArmorEffect effect)
{
	name.Create("Armor");
	subtype = ItemType::ARMOR;

	this->defense = defense;
	this->atype = atype;
	this->effect = effect;
	this->isPicked = true;

	Awake();
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