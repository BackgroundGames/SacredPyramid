#include "Accessory.h"
#include "App.h"
#include "Textures.h"

Accessory::Accessory() : Item()
{
	name.Create("Accessory");
	subtype = ItemType::ACCESSORY;

	value = 0;
	atype = AccessoryType::NO_ACCESSORY;
	effect = AccessoryEffect::NO_EFFECT;
}

Accessory::Accessory(uint value, AccessoryType atype, AccessoryEffect effect)
{
	name.Create("Accessory");
	subtype = ItemType::ACCESSORY;

	this->value = value;
	this->atype = atype;
	this->effect = effect;
	
	Awake();
}

Accessory::~Accessory()
{

}

bool Accessory::Awake()
{
	Item::Awake();

	switch (atype)
	{
	case AccessoryType::TOKEN:
		texture = app->tex->Load("Assets/Textures/Token.png");
		break;
	case AccessoryType::COLLAR:
		break;
	case AccessoryType::BRACELETS:
		break;
	case AccessoryType::BOOTS:
		break;
	case AccessoryType::CAPE:
		break;
	case AccessoryType::ARTIFACT:
		texture = app->tex->Load("Assets/Textures/Heart.png");
		break;
	case AccessoryType::NO_ACCESSORY:
		break;
	default:
		break;
	}

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
