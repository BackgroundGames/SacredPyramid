#include "Level4.h"
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

#include "Defs.h"
#include "Log.h"

Level4::Level4()
{
	name.Create("level4");
}

Level4::~Level4()
{
}

bool Level4::Awake(pugi::xml_node config)
{
	LOG("Loading Scene 3");

	sceneconfig = config;

	return true;
}

bool Level4::Start()
{
	//Get the map name from the config file and assigns the value in the module
	app->map->name = sceneconfig.child("map").attribute("name").as_string();
	app->map->path = sceneconfig.child("map").attribute("path").as_string();
	app->render->SetSelectionTex(app->tex->Load(sceneconfig.child("map").attribute("selectionPath").as_string()));
	app->map->InitMap();
	app->audio->PlayMusic(sceneconfig.attribute("audio").as_string(), 0);

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	zhaak = (Player*)app->sceneManager->previousScene->GetZhaak();
	if (zhaak == nullptr) {
		zhaak = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, PlayerType::ZHAAK);
	}
	players.push_back(zhaak);
	//SI HA CARREGAT PARTIDA MIRAR AL SAVE
	//////player->parameters = sceneconfig.child("player");
	zhaak->parameters = sceneconfig.child("zhaak");
	zhaak->Start();

	eli = (Player*)app->sceneManager->previousScene->GetEli();
	if (eli == nullptr) {
		eli = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, PlayerType::ELI);
	}
	players.push_back(eli);
	//SI HA CARREGAT PARTIDA MIRAR AL SAVE
	//////player->parameters = sceneconfig.child("player");
	eli->parameters = sceneconfig.child("eli");
	eli->Start();

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

	app->render->camera.y = 0;
	app->render->camera.x = 0;
	cameraFocus = GetPlayer();

	LockCamera();

	app->questManager->QuestCompleted(2);

	return true;
}

bool Level4::PreUpdate()
{
	return true;
}

bool Level4::Update(float dt)
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

	if (GetPlayer()->GetTile() == endTile && (GetPlayer()->exploringState == ExploringState::IDLE)) {
		GetPlayer()->exploringState = ExploringState::NONE;
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->winScreen);
	}

	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && GetPlayer()->mainState != MainState::IN_COMBAT) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && GetPlayer()->mainState != MainState::IN_COMBAT) app->LoadRequest();

	//degug
	if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->level2);

	return true;
}

bool Level4::PostUpdate()
{
	return true;
}

bool Level4::CleanUp()
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

	return true;
}

bool Level4::OnGuiMouseClickEvent(GuiControl* control) {

	if (GetPlayer()->mainState == MainState::IN_COMBAT) {
		app->entityManager->combatManager->UIEvent(control->id);
	}
	else {
		app->dialogueTree->ChoseOption(control->id);
	}

	return true;
}

void Level4::LockCamera()
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
