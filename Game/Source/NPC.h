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

	void PerformDialog();

public:
	Animation idleAnim;
	uint id;
	bool startTalking = false;
	iPoint aux;
};
#endif // !__NPC_H__

