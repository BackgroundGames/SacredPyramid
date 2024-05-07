#include "Consumable.h"
#include "App.h"
#include "Textures.h"

Consumable::Consumable()
{
	name.Create("Consumable");
	subtype = ItemType::CONSUMABLE;

	uses = 0;
	value = 0;
	ctype = ConsumableType::NO_CONSUMABLE;
	effect = ConsumableEffect::NO_EFFECT;
}

Consumable::Consumable(uint uses, uint value, ConsumableType ctype, ConsumableEffect effect)
{
	name.Create("Consumable");
	subtype = ItemType::CONSUMABLE;

	this->uses = uses;
	this->value = value;
	this->ctype = ctype;
	this->effect = effect;

	Awake();
}

Consumable::~Consumable()
{
}

bool Consumable::Awake()
{
	Item::Awake();

	switch (ctype)
	{
	case ConsumableType::POTION:
		texture = app->tex->Load("Assets/Textures/Potion.png");
		break;
	case ConsumableType::SCROLL:
		break;
	case ConsumableType::NO_CONSUMABLE:
		break;
	default:
		break;
	}
	return true;
}

bool Consumable::Start()
{
	Item::Start();
	return true;
}

bool Consumable::Update(float dt)
{
	Item::Update(dt);
	return true;
}

bool Consumable::CleanUp()
{
	Item::CleanUp();
	return true;
}
