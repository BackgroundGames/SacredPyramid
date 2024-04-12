#include "NPC.h"
#include "App.h"
#include "SceneManager.h"
#include "Player.h"
#include "DialogueTree.h"
#include "Map.h"

NPC::NPC()
{
	name.Create("npc");
	type = EntityType::NPC;
	Awake();
}

NPC::~NPC() {}

bool NPC::Awake()
{
	Character::Awake();
	return true;
}

bool NPC::Start()
{
	Character::Start();

	if (currentAnimation == nullptr) {
		idleAnim.loop = true;
		idleAnim.speed = 1.0f;
		idleAnim.PushBack({ 0, 0, 224, 246 });
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

	iPoint aux = { GetTile().x - 1,GetTile().y };

	if (GetMouseTile(mousePos) == aux && app->sceneManager->currentScene->GetPlayer()->interacted != this)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			NotifyObserver();
		}
	}	

	Character::Update(dt);

	if (startTalking) {
		app->dialogueTree->performDialogue("dialogue1");
		startTalking = false;
	}

	return true;
}

bool NPC::CleanUp()
{
	Character::CleanUp();
	return true;
}

void NPC::PerformDialog()
{
	app->dialogueTree->performDialogue("dialogue1");

	for (size_t i = 0; i < app->sceneManager->currentScene->players.size(); i++)
	{
		dynamic_cast<Player*>(app->sceneManager->currentScene->players.at(i))->exploringState = ExploringState::TALKING;
	}
}
