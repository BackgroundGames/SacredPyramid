#include "Level3.h"
#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Map.h"
#include "Item.h"
#include "DialogueTree.h"
#include "ModuleParticles.h"

#include "Defs.h"
#include "Log.h"

Level3::Level3()
{
	name.Create("level3");
}

Level3::~Level3()
{
}

bool Level3::Awake(pugi::xml_node config)
{
	LOG("Loading Scene 3");

	sceneconfig = config;

	return true;
}

bool Level3::Start()
{
	//Get the map name from the config file and assigns the value in the module
	app->map->name = sceneconfig.child("map").attribute("name").as_string();
	app->map->path = sceneconfig.child("map").attribute("path").as_string();
	app->render->SetSelectionTex(app->tex->Load(sceneconfig.child("map").attribute("selectionPath").as_string()));
	app->map->InitMap();
	app->audio->PlayMusic(sceneconfig.attribute("audio").as_string(), 0);
	rockFx = app->audio->LoadFx("Assets/Audio/Fx/Rock_press.wav");

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

	for (pugi::xml_node npcNode = sceneconfig.child("npc"); npcNode; npcNode = npcNode.next_sibling("npc"))
	{
		NPC* npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC);
		npc->id = npcNode.attribute("id").as_uint();
		npcs.push_back(npc);
		npc->parameters = npcNode;
		npc->Start();
	}

	talkedSphinx = false;

	playerPuzzle.clear();

	//The corrcet answer to the Puzzle
	correctPuzzle.push_back({ 10, 25 });		//Tile 1
	correctPuzzle.push_back({ 14, 27 });		//Tile 4
	correctPuzzle.push_back({ 17, 26 });		//Tile 7
	correctPuzzle.push_back({ 11, 27 });		//Tile 2
	correctPuzzle.push_back({ 13, 25 });		//Tile 3

	app->render->camera.y = 0;
	app->render->camera.x = 0;
	cameraFocus = GetPlayer();

	LockCamera();

	puzzlehint = app->moduleParticles->AddParticle(app->moduleParticles->laser, 18, 24);

	return true;
}

bool Level3::PreUpdate()
{
	return true;
}

bool Level3::Update(float dt)
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

	if (talkedSphinx)
	{
		if (GetPlayer()->exploringState == ExploringState::IDLE &&
			GetPlayer()->GetTile() != playerPuzzle.back())
		{
			if (GetPlayer()->GetTile() == iPoint(10, 25))
			{
				addInPuzzle(GetPlayer()->GetTile());
				app->audio->PlayFx(rockFx);
				puzzlehint->pendingToDelete = true;
				puzzlehint = app->moduleParticles->AddParticle(app->moduleParticles->laser, 14, 27);
			}

			if (GetPlayer()->GetTile() == iPoint(11, 27))
			{
				addInPuzzle(GetPlayer()->GetTile());
				app->audio->PlayFx(rockFx);
				puzzlehint->pendingToDelete = true;
				puzzlehint = app->moduleParticles->AddParticle(app->moduleParticles->laser, 13, 25);
			}

			if (GetPlayer()->GetTile() == iPoint(13, 25))
			{
				addInPuzzle(GetPlayer()->GetTile());
				app->audio->PlayFx(rockFx);
				puzzlehint->pendingToDelete = true;
				puzzlehint = app->moduleParticles->AddParticle(app->moduleParticles->laser, 18, 24);
			}
			if (GetPlayer()->GetTile() == iPoint(14, 27))
			{
				addInPuzzle(GetPlayer()->GetTile());
				app->audio->PlayFx(rockFx);
				puzzlehint->pendingToDelete = true;
				puzzlehint = app->moduleParticles->AddParticle(app->moduleParticles->laser, 17, 26);
			}

			if (GetPlayer()->GetTile() == iPoint(15, 25))
			{
				addInPuzzle(GetPlayer()->GetTile());
				app->audio->PlayFx(rockFx);
			}

			if (GetPlayer()->GetTile() == iPoint(16, 28))
			{
				addInPuzzle(GetPlayer()->GetTile());
				app->audio->PlayFx(rockFx);
			}

			if (GetPlayer()->GetTile() == iPoint(17, 26))
			{
				addInPuzzle(GetPlayer()->GetTile());
				app->audio->PlayFx(rockFx);
				puzzlehint->pendingToDelete = true;
				puzzlehint = app->moduleParticles->AddParticle(app->moduleParticles->laser, 11, 27);
			}

			if (GetPlayer()->GetTile() == iPoint(18, 28))
			{
				addInPuzzle(GetPlayer()->GetTile());
				app->audio->PlayFx(rockFx);
			}
		}
	}

	// L14: TODO 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && GetPlayer()->mainState != MainState::IN_COMBAT) app->SaveRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && GetPlayer()->mainState != MainState::IN_COMBAT) app->LoadRequest();

	if (rain == false) {
		if (GetPlayer()->GetTile() == iPoint{ 14,71 } || GetPlayer()->GetTile() == iPoint{ 15,71 }
			|| GetPlayer()->GetTile() == iPoint{ 14,32 } || GetPlayer()->GetTile() == iPoint{ 15,32 }) {
			rain = true;
			app->moduleParticles->PlayLowRain();
		}
	}
	else {
		if (GetPlayer()->GetTile() == iPoint{ 14,31 } || GetPlayer()->GetTile() == iPoint{ 15,31 }
			|| GetPlayer()->GetTile() == iPoint{ 14,72 } || GetPlayer()->GetTile() == iPoint{ 15,72 }) {
			rain = false;
			app->moduleParticles->StopWeather();
		}
	}

	return true;
}

bool Level3::PostUpdate()
{
	return true;
}

bool Level3::CleanUp()
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

	app->audio->UnloadFx(rockFx);

	if (puzzlehint != nullptr) {
		puzzlehint->pendingToDelete = true;
		puzzlehint = nullptr;
	}

	return true;
}

bool Level3::OnGuiMouseClickEvent(GuiControl* control) {

	if (GetPlayer()->mainState == MainState::IN_COMBAT) {
		app->entityManager->combatManager->UIEvent(control->id);
	}
	else {
		app->dialogueTree->ChoseOption(control->id);
	}

	return true;
}

void Level3::LockCamera()
{
	int limitCamXend = (app->map->getMapWidth() / 2 + app->map->GetTileWidth() / 2 - windowW) * -1;
	int limitCamXbeg = (app->map->getMapWidth() / 2 - app->map->GetTileWidth() / 2) *3;

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

bool Level3::CheckPuzzle()
{
	bool ret = false;

	for (size_t i = 0; i < correctPuzzle.size(); i++)
	{
		for (size_t j = 0; j < playerPuzzle.size(); j++)
		{
			if (correctPuzzle.at(i) == playerPuzzle.at(j))
				ret = true;
			else
				ret = false;
		}
	}

	playerPuzzle.clear();

	return ret;
}

void Level3::addInPuzzle(iPoint p)
{
	bool isInPuzzle = false;

	for (size_t i = 0; i < playerPuzzle.size(); i++)
	{
		if (playerPuzzle.at(i) == p)
			isInPuzzle = true;	
	}

	if (!isInPuzzle)
		playerPuzzle.push_back(p);
}
