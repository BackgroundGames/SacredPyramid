#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"

enum class EntityType
{
	//CHARCTER types
	PLAYER,
	ENEMY,
	NPC,
	
	//ITEM types
	ITEM,

	UNKNOWN
};

enum class EnemyType
{
	BANDIT,
	DRUNKARD,
	EVILMUMMY
};

enum class ItemType
{
	WEAPON,
	ARMOR,
	ACCESSORY
};

class PhysBody;

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}
	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool OnGuiMouseClickEvent(Entity* control) {
		return true;
	};

	void NotifyObserver()
	{
		if (observer != nullptr) {
			observer->OnGuiMouseClickEvent(this);
		}
	}

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters; 
	
	SDL_Texture* texture = NULL;
	uint texH, texW;

	iPoint position;      

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	bool renderable = true;

	Entity* observer;
};

#endif // __ENTITY_H__