#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void PlaceItem(int x, int y);

public:
	uint id = 0;
	bool isPicked = false;
	bool isDropped = false;
	SDL_Texture* texture;
	ItemType subtype;

private:
	uint texW, texH;

	//debug
	SDL_Rect prect;
};

#endif // __ITEM_H__