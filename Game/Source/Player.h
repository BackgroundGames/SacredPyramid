#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Character.h"


class Player : public Character
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:
	bool lastStand = false;
};

#endif // __PLAYER_H__