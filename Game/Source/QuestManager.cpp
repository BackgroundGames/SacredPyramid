#include "QuestManager.h"

#include "App.h"
#include "Render.h"
#include "Window.h"
#include "Audio.h"

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
    app->win->GetWindowSize(winW,winH);
    questComplete = app->audio->LoadFx("Assets/Audio/Fx/Quest complete.wav");
    return true;
}

bool QuestManager::Update(float dt)
{
    int margin = 0;

    if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) {
        showQuests = !showQuests;
    }

    if (showQuests) {
        for (size_t i = 0; i < activeQuests.size(); i++)
        {
            if (i != 0) {
                margin = 5;
            }
            SDL_Rect quat = { winW - activeQuests.at(i)->questName.Length() * 20 - 5,
            i * 70 + margin * i, activeQuests.at(i)->questName.Length() * 20 + 5, 70 };
            app->render->DrawRectangle(quat, 20, 20, 20, 175, true, false);

            int r = 255; int g = 255; int b = 255;
            if (activeQuests.at(i)->completed) {
                r = 175; g = 175; b = 175;
            }

            app->render->DrawText(activeQuests.at(i)->questName.GetString(), quat.x, quat.y, quat.w, 50, r, g, b);
            app->render->DrawText(activeQuests.at(i)->questDescription.GetString(), winW - activeQuests.at(i)->questDescription.Length() * 10,
                quat.y + 50, activeQuests.at(i)->questDescription.Length() * 10, 20, r, g, b);

            activeQuests.at(i)->Update(dt);
        }
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

Quest* QuestManager::AddQuest(int type, int id)
{
    Quest* quest;

    if (type == 0) {
        quest = new TalkTo(id);
    }

    activeQuests.push_back(quest);

    return quest;
}

void QuestManager::QuestCompleted(int id)
{
    for (size_t i = 0; i < activeQuests.size(); i++)
    {
        if (activeQuests.at(i)->id == id) {
            activeQuests.at(i)->questDescription = "completed";
            activeQuests.at(i)->completed = true;
            app->audio->PlayFx(questComplete);
        }
    }
}
