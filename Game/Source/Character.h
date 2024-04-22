#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Entity.h"
#include "SDL/include/SDL.h"
#include "DynArray.h"


#include "Weapon.h"
#include "Armor.h"
#include "Accessory.h"
#include "Animation.h"

struct Stats
{
	int health = 1;
	uint mana = 0;
	uint attack = 1;
	uint defense = 0;
	uint magicAttack = 0;
	uint magicDefense = 0;
	uint initiative = 0;
	uint movement = 3;
};

struct Equipment
{
	Weapon weapon;
	Armor armor;
	Accessory accessory;
};

enum class MainState
{
	OUT_OF_COMBAT,
	IN_COMBAT,
	NONE
};

enum class CombatState
{
	WAITING,
	IDLE,
	MOVING,
	ATTACKING,
	DEAD,
	MOVEMENT,
	NONE
};

enum class ExploringState
{
	IDLE,
	MOVING,
	FOLLOWING,
	TALKING,
	INTERACT,
	NONE
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

	bool moveTo(iPoint destination);

	void DoPathMoving();

	void DebugPath();

	void ResetPath();

	iPoint GetTile();
	iPoint GetMouseTile(iPoint mousePos);

	uint GetHealth() { return this->stats.health; }

	uint GetMana() { return this->stats.mana; }

	uint GetAttack() { return this->stats.attack; }

	uint GetDefense() { return this->stats.defense; }

	uint GetMagicAttack() { return this->stats.magicAttack; }

	uint GetMagicDefense() { return this->stats.magicDefense; }

	uint GetInitiative() { return this->stats.initiative; }

	uint GetMovement() { return this->stats.movement; }

	uint DistanceToTile(iPoint Tile1, iPoint Tile2);

public:
	//Declare character parameters
	Stats stats;
	Equipment inventory;
	float speed = 0.2f;
	float velocity = 1;

	iPoint mousePos = iPoint{ 0, 0 };

	uint movementUsed = 0;
	uint aviableMovement = 0;

	//to move
	DynArray<iPoint> path;
	int pathingIteration = 0;
	bool move = false;
	fPoint translationOffset = fPoint{ 0.0f, 0.0f };
	iPoint auxPosition = iPoint{ 0, 0 };
	iPoint prevDestination = iPoint{ 0, 0 };
	Direction PosState;

	//debug
	SDL_Rect prect;
	SDL_Texture* selectionTex = NULL;

	//anim
	Animation* currentAnimation = nullptr;
	Animation idleAnim;
	Animation idleAnimB;
	Animation walkingAnim;

	//player stats
	MainState mainState;
	CombatState combatState;
	ExploringState exploringState;
	ExploringState previousEState;

	int sandFx;
	int sandChannel;
};


#endif // !__CHARACTER_H__

