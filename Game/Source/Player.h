#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "DynArray.h"

enum class Direction {
	UL,
	UR,
	DL,
	DR
};

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void TpToCell(int x, int y);

	void moveTo(iPoint destination);

	void DoPathMoving();

	void DoPathTping();

	iPoint GetTile();

public:

	//Declare player parameters
	float speed = 0.2f;
	SDL_Texture* texture = NULL;
	pugi::xml_node config;
	uint texW, texH;

	//Audio fx
	SDL_Rect prect;
	int pickCoinFxId;

	int pathingIteration = 0;
	bool move = false;
	bool finishMoving = false;
	fPoint translationOffset;
	iPoint auxPosition;
	iPoint prevDestination;
	Direction PosState;

	//Tile selection
	SDL_Texture* selectionTex = NULL;
};

#endif // __PLAYER_H__