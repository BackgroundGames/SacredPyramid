#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Entity.h"
#include "Item.h"

enum class WeaponType {
	SWORD,
	BOOK,
	SHIELD,
	WRAPS,
	DAGGER,
	FISTS,
	NO_WEAPON
};

enum class WeaponEffect {
	POISON,
	PIERCE,
	MAGIC,
	ELEMENT,
	NO_EFFECT
};

class Weapon : public Item
{
public:
	Weapon();
	Weapon(int range, int damage, WeaponType wtype, WeaponEffect effect);

	virtual ~Weapon();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:
	WeaponType wtype;
	WeaponEffect effect;
	int range;
	int damage;
};

#endif // __WEAPON_H__