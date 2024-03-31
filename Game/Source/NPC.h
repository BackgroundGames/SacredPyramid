#ifndef __NPC_H__
#define __NPC_H__

#include "Character.h"

class NPC : public Character
{
public:

	NPC();
	
	virtual ~NPC();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:
	Animation idleAnim;
	uint id;
};
#endif // !__NPC_H__

