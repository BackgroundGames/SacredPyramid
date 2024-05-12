#include "QuestManager.h"

#include "App.h"
#include "Render.h"

QuestManager::QuestManager() : Module()
{
    name.Create("questmanager");
}

QuestManager::~QuestManager()
{

}

bool QuestManager::Awake(pugi::xml_node config)
{
    return true;
}

bool QuestManager::Start()
{
    return true;
}

bool QuestManager::Update(float dt)
{
    for (size_t i = 0; i < activeQuests.size(); i++)
    {
        app->render->DrawText(name.GetString(), 0, 0, 100, 50, 255, 255, 255);
        activeQuests.at(i)->Update(dt);
    }
   
    return true;
}

bool QuestManager::PostUpdate()
{
    return true;
}

bool QuestManager::CleanUp()
{
    return true;
}

Quest* QuestManager::AddQuest(int id)
{
    Quest* quest;

    if (id == 0) {
        quest = new TalkTo();
    }

    activeQuests.push_back(quest);

    return quest;
}
