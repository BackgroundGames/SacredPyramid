#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Entity.h"
#include "Item.h"

enum WeaponType {
	SWORD,
	BOOK,
	DAGGER
};

enum WeaponEffect {
	POISON,
	PIERCE,
	MAGIC
};

class Weapon : public Item
{
public:
	Weapon();

	virtual ~Weapon();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:
	WeaponType wtype;
	WeaponEffect effect;
};

#endif // __WEAPON_H__