#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "Module.h"
#include "Quest.h"
#include <vector>

using namespace std;

class QuestManager : public Module
{
public:

	QuestManager();

	// Destructor
	~QuestManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	Quest* AddQuest(int id);

public:
	pugi::xml_node config;
	vector<Quest*> activeQuests;
};

#endif //__QUESTMANAGER_H__
