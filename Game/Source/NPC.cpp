#include "NPC.h"
#include "App.h"
#include "SceneManager.h"
#include "Player.h"
#include "DialogueTree.h"
#include "Map.h"

NPC::NPC()
{
	name.Create("NPC");
	type = EntityType::NPC;
}

NPC::~NPC() {}

bool NPC::Awake()
{
	return true;
}

bool NPC::Start()
{
	Character::Start();

	if (currentAnimation == nullptr) {
		idleAnim.loop = true;
		idleAnim.speed = 1.0f;
		idleAnim.PushBack({ 0, 0, 56, 124 });
	}

	currentAnimation = &idleAnim;
	texW = currentAnimation->GetCurrentFrame().w;
	texH = currentAnimation->GetCurrentFrame().h;

	TpToCell(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	observer = (Entity*) app->sceneManager->currentScene->GetPlayer(); 

	return true;
}

bool NPC::Update(float dt)
{
	distanceFromPlayer = DistanceToTile(GetTile(), app->sceneManager->currentScene->GetPlayer()->GetTile());

	if (GetMouseTile(mousePos) == GetTile() && app->sceneManager->currentScene->GetPlayer()->exploringState != ExploringState::TALKING)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			NotifyObserver();
		}
	}

	if (distanceFromPlayer <= 1)
	{
		app->dialogueTree->performDialogue("dialogue1");

		for (size_t i = 0; i < app->sceneManager->currentScene->players.Count(); i++)
		{
			dynamic_cast<Player*>(app->sceneManager->currentScene->players.At(i)->data)->exploringState = ExploringState::TALKING;
		}
	}
	

	Character::Update(dt);

	return true;
}

bool NPC::CleanUp()
{
	Character::CleanUp();
	return true;
}
