#ifndef __ACCESSORY_H__
#define __ACCESSORY_H__

#include "Entity.h"
#include "Item.h"

class Accessory : public Item
{
public:
	Accessory();

	virtual ~Accessory();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();
public:

};

#endif // __ACCESSORY_H__