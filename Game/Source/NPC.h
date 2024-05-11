#ifndef __NPC_H__
#define __NPC_H__

#include "Character.h"

class Quest;

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
	
	void QuestAccepted();

public:
	Animation idleAnim;
	uint id;
	bool startTalking = false;
	iPoint aux;
	Quest* quest = nullptr;
};
#endif // !__NPC_H__

