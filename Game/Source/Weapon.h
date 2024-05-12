#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "Entity.h"
#include "Item.h"

using namespace std;

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
	Weapon(int range, int damage, string wtype, string effect);

	virtual ~Weapon();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:
	string wtype;
	string effect;
	int range;
	int damage;
};

#endif // __WEAPON_H__