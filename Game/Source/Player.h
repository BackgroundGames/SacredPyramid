#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Character.h"

enum class PlayerType
{
	UNKNOWN,

	ZHAAK,
	AMIR,
	AMUMMY,
	ELI
};

class Player : public Character
{
public:

	Player();
	Player(PlayerType subtype);

	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool OnGuiMouseClickEvent(Entity* control);

	iPoint GetMouseTile(iPoint mousePos);

public:
	iPoint mousePos = iPoint{ 0, 0 };
	iPoint mouseTile = iPoint{ 0, 0 };

	bool lastStand = false;
	PlayerType subtype;
};

#endif // __PLAYER_H__