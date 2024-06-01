#include "Level1.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Item.h"
#include "DialogueTree.h"
#include "QuestManager.h"
#include "ModuleParticles.h"

#include "Defs.h"
#include "Log.h"

Level1::Level1() : Scene()
{
	name.Create("level1");
}

Level1::~Level1()
{
}

bool Level1::Awake(pugi::xml_node config)
{
	LOG("Loading Scene");
	bool ret = true;

	sceneconfig = config;

	return ret;
}

bool Level1::Start()
{
	//Get the map name from the config file and assigns the value in the module
	
	app->audio->PlayMusic(sceneconfig.attribute("audio").as_string(), 0);

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	if (app->sceneManager->previousScene != (Scene*)app->sceneManager->menu &&
		app->sceneManager->previousScene != (Scene*)app->sceneManager->intro) {

		app->map->name = "Mapa 1.75 desierto.tmx";
		app->map->path = "Assets/Maps/";
		app->render->SetSelectionTex(app->tex->Load("Assets/Maps/tileSelectionIso128.png"));
		app->map->InitMap();

		zhaak = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, PlayerType::ZHAAK);
		players.push_back(zhaak);
		//Assigns the XML node to a member in player
		zhaak->Start();
		zhaak->TpToCell(17,25);

		if (eli == nullptr) {
			eli = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, PlayerType::ELI);
		}
		players.push_back(eli);
		eli->Start();
		eli->TpToCell(16, 25);
		eli->exploringState = ExploringState::FOLLOWING;


		// iterate all entities in the scene --> Check https://pugixml.org/docs/quickstart.html#access
		for (pugi::xml_node enemyNode = sceneconfig.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
		{
			Enemy* enemy = nullptr;

			switch (enemyNode.attribute("id").as_uint())
			{
			case 0:
				enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY, PlayerType::UNKNOWN, EnemyType::BANDIT);
				break;
			case 1:
				enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY, PlayerType::UNKNOWN, EnemyType::DRUNKARD);
				break;
			case 2:
				enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY, PlayerType::UNKNOWN, EnemyType::EVIL_MUMMY);
				break;
			default:
				break;
			}

			enemy->id = enemyNode.attribute("id").as_uint();
			enemies.push_back(enemy);
			enemy->parameters = enemyNode;
			enemy->Start();
		}

		nextMap = app->moduleParticles->AddParticle(app->moduleParticles->laser, puenteTile.x, puenteTile.y);

	}
	else {

		app->map->name = sceneconfig.child("map").attribute("name").as_string();
		app->map->path = sceneconfig.child("map").attribute("path").as_string();
		app->render->SetSelectionTex(app->tex->Load(sceneconfig.child("map").attribute("selectionPath").as_string()));
		app->map->InitMap();

		eli = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, PlayerType::ELI);
		players.push_back(eli);
		//Assigns the XML node to a member in player
		eli->parameters = sceneconfig.child("eli");
		eli->Start();

		for (pugi::xml_node npcNode = sceneconfig.child("npc"); npcNode; npcNode = npcNode.next_sibling("npc"))
		{
			NPC* npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC);
			npc->id = npcNode.attribute("id").as_uint();
			npcs.push_back(npc);
			npc->parameters = npcNode;
			npc->Start();
		}

		app->dialogueTree->performDialogue("dialogue0");
		GetPlayer()->exploringState = ExploringState::TALKING;
		app->questManager->AddQuest(0, 0);

		/*for (pugi::xml_node itemNode = sceneconfig.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
		{
			Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
			item->parameters = itemNode;
		}*/

		nextMap = app->moduleParticles->AddParticle(app->moduleParticles->laser, tabernTile.x, tabernTile.y);
	}

	app->render->camera.y = 0;
	app->render->camera.x = 0;
	cameraFocus = GetPlayer();

	LockCamera();

	app->moduleParticles->PlaySandBreeze();

	return true;
}

bool Level1::PreUpdate()
{
	return true;
}

bool Level1::Update(float dt)
{
	LockCamera();
	//Make the camera movement independent of framerate
	float camSpeed = 1;

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y -= (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y += (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x -= (int)ceil(camSpeed * dt);

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x += (int)ceil(camSpeed * dt);

	if (GetPlayer()->GetTile() == tabernTile && (GetPlayer()->exploringState == ExploringState::IDLE)) {
		GetPlayer()->exploringState = ExploringState::NONE;
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->level2);
	}

	if (GetPlayer()->GetTile() == puenteTile && (GetPlayer()->exploringState == ExploringState::IDLE)) {
		GetPlayer()->exploringState = ExploringState::NONE;
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->level3);
	}

	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && GetPlayer()->mainState != MainState::IN_COMBAT) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && GetPlayer()->mainState != MainState::IN_COMBAT) app->LoadRequest();
	
	return true;
}

bool Level1::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool Level1::CleanUp()
{
	if (app->sceneManager->newScene->sceneType == MENU ||
		app->sceneManager->newScene->sceneType == WIN_SCREEN ||
		app->sceneManager->newScene->sceneType == LOSE_SCREEN) 
	{
		for (size_t i = 0; i < players.size(); i++)
		{
			players[i]->CleanUp();
			app->entityManager->DestroyEntity((Entity*)players[i]);
			delete players[i];
		}
		zhaak = nullptr;
		eli = nullptr;
	}
	players.clear();

	for (size_t i = 0; i < enemies.size(); i++)
	{
		enemies[i]->CleanUp();
		app->entityManager->DestroyEntity((Entity*)enemies[i]);
		delete enemies[i];
	}
	enemies.clear();

	for (size_t i = 0; i < npcs.size(); i++)
	{
		npcs[i]->CleanUp();
		app->entityManager->DestroyEntity((Entity*)npcs[i]);
		delete npcs[i];
	}
	npcs.clear();

	cameraFocus = nullptr;

	if (nextMap != nullptr) {
		nextMap->pendingToDelete = true;
	}

	app->moduleParticles->StopWeather();

	return true;
}

bool Level1::OnGuiMouseClickEvent(GuiControl* control) {

	if (GetPlayer()->mainState == MainState::IN_COMBAT) {
		app->entityManager->combatManager->UIEvent(control->id);
	}
	else {
		app->dialogueTree->ChoseOption(control->id);
	}

	return true;
}

void Level1::LockCamera()
{
	int limitCamXend = (app->map->getMapWidth() / 2 + app->map->GetTileWidth() / 2 - windowW) * -1;
	int limitCamXbeg = (app->map->getMapWidth() / 2 - app->map->GetTileWidth() / 2);

	app->render->camera.y = ((cameraFocus->position.y - cameraFocus->texW / 2) - (windowH / 2)) * -1;
	app->render->camera.x = ((cameraFocus->position.x - cameraFocus->texH / 2) - (windowW / 2)) * -1;

	if (app->render->camera.x > limitCamXbeg) {
		app->render->camera.x = limitCamXbeg;
	}
	else if (app->render->camera.x < limitCamXend) {
		app->render->camera.x = limitCamXend;
	}

	int limitCamYend = (app->map->getMapHeght() + app->map->GetTileHeight() / 2 - windowH) * -1;
	int limitCamYbeg = app->map->GetTileHeight() / 2 * -1;

	if (app->render->camera.y > limitCamYbeg) {
		app->render->camera.y = limitCamYbeg;
	}
	else if (app->render->camera.y < limitCamYend) {
		app->render->camera.y = limitCamYend;
	}
}
