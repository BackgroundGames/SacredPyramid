#ifndef __ARMOR_H__
#define __ARMOR_H__

#include "Entity.h"
#include "Item.h"

enum ArmorEffect {
	THORNS
};

class Armor : public Item
{
public:
	Armor();

	virtual ~Armor();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
public:
	ArmorEffect effect;
};

#endif // __ARMOR_H__