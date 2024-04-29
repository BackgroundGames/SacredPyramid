#ifndef __QuestItem_H__
#define __QuestItem_H__

#include "Entity.h"
#include "Item.h"

class QuestItem : public Item
{
public:
	QuestItem();

	virtual ~QuestItem();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

};

#endif // __QuestItem_H__