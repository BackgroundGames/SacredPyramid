#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "DynArray.h"

struct Stats
{
	uint health;
	uint mana;
	uint attack;
	uint defense;
	uint magicAttack;
	uint magicDefense;
	uint initiative;
};

struct Equipment
{
	//Weapon weapon;
	//Armor armor;
	//Accessory accessory;
};

enum class Direction {
	UL,
	UR,
	DL,
	DR
};

struct SDL_Texture;

class Character : public Entity
{
public:

	Character();

	virtual ~Character();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void TpToCell(int x, int y);

	void moveTo(iPoint destination);

	void DoPathMoving();

	iPoint GetTile();

	uint GetHealth() { return this->stats.health; }

	uint GetMana() { return this->stats.mana; }

	uint GetAttack() { return this->stats.attack; }

	uint GetDefense() { return this->stats.defense; }

	uint GetMagicAttack() { return this->stats.magicAttack; }

	uint GetMagicDefense() { return this->stats.magicDefense; }

	uint GetInitiative() { return this->stats.initiative; }

public:

	//Declare character parameters
	Stats stats;
	Equipment inventory;
	float speed = 0.2f;

	//Animation
	//Animation* currentAnim;

	//to move
	const DynArray<iPoint>* path;
	int pathingIteration = 0;
	bool move = false;
	bool finishMoving = false;
	fPoint translationOffset;
	iPoint auxPosition;
	iPoint prevDestination;
	Direction PosState;

	//debug
	SDL_Rect prect;
	SDL_Texture* selectionTex = NULL;
};


#endif // !__CHARACTER_H__

