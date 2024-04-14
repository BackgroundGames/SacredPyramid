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
	id = parameters.attribute("id").as_uint();

	if (id == 0) {
		parametersAnim = app->configFile.child("config").child("animations").child("zhaaknpc");
	}
	if (id == 1) {
		parametersAnim = app->configFile.child("config").child("animations").child("random");
	}
	if (id == 2) {
		parametersAnim = app->configFile.child("config").child("animations").child("tabernero");
	}

	Character::Start();

	if (currentAnimation == nullptr) {
		pugi::xml_node auxAnim = parametersAnim.child("idleAnim");

		idleAnim.speed = auxAnim.attribute("speed").as_float();
		idleAnim.loop = auxAnim.attribute("loop").as_bool();

		for (pugi::xml_node idleNode = auxAnim.child("idle"); idleNode; idleNode = idleNode.next_sibling("idle"))
		{
			idleAnim.PushBack({ idleNode.attribute("x").as_int(), idleNode.attribute("y").as_int() ,idleNode.attribute("w").as_int() ,idleNode.attribute("h").as_int() });
		}
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

	iPoint aux = { GetTile().x,GetTile().y };

	if (GetMouseTile(mousePos) == aux && app->sceneManager->currentScene->GetPlayer()->interacted != this)
	{
		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		{
			NotifyObserver();
		}
	}	

	Character::Update(dt);

	if (startTalking) {
		if (id == 0) {
			app->dialogueTree->performDialogue("dialogue4");
		}
		if (id == 1) {
			app->dialogueTree->performDialogue("dialogue1");
		}
		if (id == 2) {
			app->dialogueTree->performDialogue("dialogue2");
		}
		
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
