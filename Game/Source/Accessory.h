#ifndef __ACCESSORY_H__
#define __ACCESSORY_H__

#include "Entity.h"
#include "Item.h"

enum class AccessoryType
{
	TOKEN,
	COLLAR,
	BRACELETS,
	BOOTS,
	CAPE,
	ARTIFACT,
	NO_ACCESSORY
};

enum class AccessoryEffect
{
	STAT_BONUS,
	HEAL,
	CAST_SPELL,
	NO_EFFECT
};

class Accessory : public Item
{
public:
	Accessory();
	Accessory(uint value, AccessoryType atype, AccessoryEffect effect);

	virtual ~Accessory();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:
	uint value;
	AccessoryType atype;
	AccessoryEffect effect;
};

#endif // __ACCESSORY_H__