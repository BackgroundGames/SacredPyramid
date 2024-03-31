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
	app->map->name = sceneconfig.child("map").attribute("name").as_string();
	app->map->path = sceneconfig.child("map").attribute("path").as_string();
	app->render->SetSelectionTex(app->tex->Load(sceneconfig.child("map").attribute("selectionPath").as_string()));
	app->map->InitMap();

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	if (app->sceneManager->previousScene != (Scene*)app->sceneManager->menu &&
		app->sceneManager->previousScene != (Scene*)app->sceneManager->intro) {
		players.Add(zhaak);
		zhaak->parameters = sceneconfig.child("zhaak");
		zhaak->Start();

		players.Add(eli);
		eli->parameters = sceneconfig.child("eli");
		eli->Start();
	}
	else {
		//Instantiate the player using the entity manager
		//Get player paremeters
		zhaak = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, PlayerType::ZHAAK);
		players.Add(zhaak);
		//Assigns the XML node to a member in player
		zhaak->parameters = sceneconfig.child("zhaak");
		zhaak->Start();

		eli = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, PlayerType::ELI);
		players.Add(eli);
		//Assigns the XML node to a member in player
		eli->parameters = sceneconfig.child("eli");
		eli->Start();

		// iterate all entities in the scene --> Check https://pugixml.org/docs/quickstart.html#access
		for (pugi::xml_node enemyNode = sceneconfig.child("enemy"); enemyNode; enemyNode = enemyNode.next_sibling("enemy"))
		{
			Enemy* enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
			enemy->id = enemyNode.attribute("id").as_uint();
			enemies.Add(enemy);
			enemy->parameters = enemyNode;
			enemy->Start();
		}

		for (pugi::xml_node npcNode = sceneconfig.child("npc"); npcNode; npcNode = npcNode.next_sibling("npc"))
		{
			NPC* npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC);
			npc->id = npcNode.attribute("id").as_uint();
			npcs.Add(npc);
			npc->parameters = npcNode;
			npc->Start();
		}

		/*for (pugi::xml_node itemNode = sceneconfig.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
		{
			Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
			item->parameters = itemNode;
		}*/
	}

	app->render->camera.y = 0;
	app->render->camera.x = 0;

	LockCamera();

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

	//DEBUG: Change Scenes forward and backward
	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->level2);

	if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->menu);
	
	return true;
}

bool Level1::PostUpdate()
{
	bool ret = true;

	//if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		//ret = false;

	return ret;
}

bool Level1::CleanUp()
{
	if (app->sceneManager->newScene == (Scene*)app->sceneManager->menu) {
		for (size_t i = 0; i < players.Count(); i++)
		{
			players[i]->CleanUp();
			app->entityManager->DestroyEntity((Entity*)players[i]);
			delete players[i];
		}
		zhaak = nullptr;
		eli = nullptr;
	}

	for (size_t i = 0; i < enemies.Count(); i++)
	{
		if (enemies[i] != nullptr) {
			enemies[i]->CleanUp();
			app->entityManager->DestroyEntity((Entity*)enemies[i]);
			delete enemies[i];
		}
	}
	enemies.Clear();


	players.Clear();

	return true;
}

bool Level1::OnGuiMouseClickEvent(GuiControl* control) {

	app->dialogueTree->ChoseOption(control->id);

	return true;
}

void Level1::LockCamera()
{
	int limitCamXend = (app->map->getMapWidth() / 2 + app->map->GetTileWidth() / 2 - windowW) * -1;
	int limitCamXbeg = (app->map->getMapWidth() / 2 - app->map->GetTileWidth() / 2);

	app->render->camera.y = ((zhaak->position.y - zhaak->texW / 2) - windowH / 2) * -1;
	app->render->camera.x = ((zhaak->position.x - zhaak->texH / 2) - (windowW / 2)) * -1;

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
