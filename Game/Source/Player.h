#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Character.h"

class Player : public Character
{
public:

	Player();

	virtual ~Player();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool OnGuiMouseClickEvent(Entity* control);

public:
	iPoint destination = iPoint{ 0, 0 };
	PlayerType subtype;

	bool lastStand = false;
	Entity* interacted = nullptr;
};

#endif // __PLAYER_H__