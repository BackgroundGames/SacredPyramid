#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

enum class ItemType
{
	WEAPON,
	ARMOR,
	ACCESSORY,

	UNKNOWN
};

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

public:

	bool isPicked = false;
	ItemType subtype;

private:

	SDL_Texture* texture;
	const char* texturePath;
	uint texW, texH;
};

#endif // __ITEM_H__