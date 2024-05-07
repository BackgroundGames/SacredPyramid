#ifndef __CONSUMABLE_H__
#define __CONSUMABLE_H__

#include "Entity.h"
#include "Item.h"

enum class ConsumableType
{
	POTION,
	SCROLL,
	NO_CONSUMABLE
};

enum class ConsumableEffect
{
	HEAL,
	DAMAGE,
	CAST_SPELL,
	NO_EFFECT
};

class Consumable : public Item
{
public:
	Consumable();
	Consumable(uint uses, uint value, ConsumableType ctype, ConsumableEffect effect);

	virtual ~Consumable();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:
	uint uses;
	uint value;
	ConsumableType ctype;
	ConsumableEffect effect;
};

#endif // __CONSUMABLE_H__