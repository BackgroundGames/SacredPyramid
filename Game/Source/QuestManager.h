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

	Quest* AddQuest(int type, int id);

	void QuestCompleted(int id);

public:
	pugi::xml_node config;
	vector<Quest*> activeQuests;
	uint winW, winH;
	bool showQuests = true;

	int questComplete;
};

#endif //__QUESTMANAGER_H__
