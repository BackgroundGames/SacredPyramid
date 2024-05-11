#ifndef __QUEST_H__
#define __QUEST_H__

#include "PugiXml/src/pugixml.hpp"
#include "SString.h"

enum QuestType
{
	TALKTO,
	GETITEM,
	NONE
};

class Quest
{
	Quest();

	// Destructor
	virtual ~Quest();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start() {
		return false;
	};

	// Called every frame
	virtual bool Update(float dt) {
		return false;
	};

	// Called before quitting
	virtual bool CleanUp() {
		return false;
	};

public:
	SString questName;
	SString questDescription;
	QuestType questType;
	pugi::xml_node config;
};

class Character;

class TalkTo : public Quest
{
	TalkTo();

	// Destructor
	~TalkTo();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

public:
	Character* interacted;
};

#endif //__QUEST_H__