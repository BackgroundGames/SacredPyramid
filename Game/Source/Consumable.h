#ifndef __CONSUMABLE_H__
#define __CONSUMABLE_H__

#include "Entity.h"
#include "Item.h"

class Consumable : public Item
{
public:
	Consumable();

	virtual ~Consumable();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

};

#endif // __CONSUMABLE_H__