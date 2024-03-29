#include "Level2.h"
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

Level2::Level2() : Scene()
{
	name.Create("level2");
}

Level2::~Level2()
{
}

bool Level2::Awake(pugi::xml_node config)
{
	sceneconfig = config;
	return true;
}

bool Level2::Start()
{
	//Get the map name from the config file and assigns the value in the module
	app->map->name = sceneconfig.child("map").attribute("name").as_string();
	app->map->path = sceneconfig.child("map").attribute("path").as_string();
	app->render->SetSelectionTex(app->tex->Load(sceneconfig.child("map").attribute("selectionPath").as_string()));
	app->map->InitMap();

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Instantiate the player using the entity manager
	//Get player paremeters
	player = app->sceneManager->previousScene->GetPlayer();
	//SI HA CARREGAT PARTIDA MIRAR AL SAVE
	//////player->parameters = sceneconfig.child("player");
	player->parameters = sceneconfig.child("player");
	player->Start();

	/*enemy = (Enemy*)app->entityManager->CreateEntity(EntityType::ENEMY);
	enemy->parameters = sceneconfig.child("enemy");
	enemy->Start();*/

	// iterate all items in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	/*for (pugi::xml_node itemNode = sceneconfig.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}*/

	app->render->camera.y = 0;
	app->render->camera.x = 0;
	LockCamera();

	return true;
}

bool Level2::PreUpdate()
{
	return true;
}

bool Level2::Update(float dt)
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

	//Implement a method that repositions the player in the map with a mouse click


	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		app->dialogueTree->performDialogue("dialogue1");

	return true;
}

bool Level2::PostUpdate()
{
	return true;
}

bool Level2::CleanUp()
{
	return true;
}

bool Level2::OnGuiMouseClickEvent(GuiControl* control)
{
	app->dialogueTree->ChoseOption(control->id);
	return true;
}

void Level2::LockCamera()
{
	int limitCamXend = (app->map->getMapWidth() / 2 + app->map->GetTileWidth() / 2 - windowW) * -1;
	int limitCamXbeg = (app->map->getMapWidth() / 2 - app->map->GetTileWidth() / 2);

	app->render->camera.y = ((player->position.y - 26 / 2) - windowH / 2) * -1;
	app->render->camera.x = ((player->position.x - 40 / 2) - (windowW / 2)) * -1;

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