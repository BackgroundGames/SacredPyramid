#ifndef __ARMOR_H__
#define __ARMOR_H__

#include "Entity.h"
#include "Item.h"

enum class ArmorType {
	LEATHER,
	CHAINSHIRT,
	SCALE,
	HALFPLATE,
	FULLPLATE,
	NO_ARMOR
};

enum class ArmorEffect {
	THORNS,
	NO_EFFECT
};

class Armor : public Item
{
public:
	Armor();
	Armor(int defense, ArmorType atype, ArmorEffect effect);

	virtual ~Armor();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
public:
	ArmorType atype;
	ArmorEffect effect;
	int defense = 0;
};

#endif // __ARMOR_H__