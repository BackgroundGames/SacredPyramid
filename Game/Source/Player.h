#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Character.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "DynArray.h"

struct SDL_Texture;

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