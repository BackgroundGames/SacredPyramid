#include "SceneManager.h"

#include "App.h"
#include "Window.h"
#include "GuiManager.h"
#include "Settings.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Intro.h"
#include "GameTitle.h"
#include "Menu.h"
#include "GamePause.h"
#include "WinScreen.h"
#include "LoseScreen.h"
#include "InventoryMenu.h"
#include "Credits.h"
#include "Map.h"


SceneManager::SceneManager()
{
	name.Create("scenemanager");

	intro = new Intro();
	intro->sceneType = SceneType::INTRO;
	gameTitle = new GameTitle();
	gameTitle->sceneType = SceneType::GAME_TITLE;
	menu = new Menu();
	menu->sceneType = SceneType::MENU;
	credits = new Credits();
	credits->sceneType = SceneType::CREDITS;
	winScreen = new WinScreen();
	winScreen->sceneType = SceneType::WIN_SCREEN;
	loseScreen = new LoseScreen();
	loseScreen->sceneType = SceneType::LOSE_SCREEN;
	level1 = new Level1();
	level1->sceneType = SceneType::LEVEL1;
	level2 = new Level2();
	level2->sceneType = SceneType::LEVEL2;
	level3 = new Level3();
	level3->sceneType = SceneType::LEVEL3;
	level4 = new Level4();
	level4->sceneType = SceneType::LEVEL4;
	settings = new Settings();
	gamePause = new GamePause();
	inventoryMenu = new InventoryMenu();

	scenes.Add(intro);
	scenes.Add(gameTitle);
	scenes.Add(menu);
	scenes.Add(credits);
	scenes.Add(winScreen);
	scenes.Add(loseScreen);
	scenes.Add(level1);
	scenes.Add(level2);
	scenes.Add(level3);
	scenes.Add(level4);
	scenes.Add(inventoryMenu);
	scenes.Add(gamePause);
	
	scenes.Add(settings);
}

SceneManager::~SceneManager()
{

}

bool SceneManager::Awake(pugi::xml_node config)
{
	app->win->GetWindowSize(windowW, windowH);

	screenRect = { 0, 0, (int)windowW, (int)windowH };

	configScenes = config;

	bool ret = true;

	ListItem<Scene*>* item;
	Scene* pScene = NULL;

	for (item = scenes.start; item != NULL; item = item->next)
	{
		pScene = item->data;

		ret = item->data->Awake(config.child(item->data->name.GetString()));
	}
	return ret;
}

bool SceneManager::Start()
{
	ChangeScane(intro);

	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	return true;
}

bool SceneManager::PreUpdate()
{
	return true;
}

bool SceneManager::Update(float dt)
{
	//OPTICK_EVENT();

	bool ret = true;
	ListItem<Scene*>* item;
	Scene* pScene = NULL;

	for (item = scenes.start; item != NULL; item = item->next)
	{
		pScene = item->data;

		if (pScene->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	if (fade) {
		MakeFade();
	}

	return ret;
}

bool SceneManager::PostUpdate()
{
	bool ret = true;
	ListItem<Scene*>* item;
	Scene* pScene = NULL;

	for (item = scenes.start; item != NULL; item = item->next)
	{
		pScene = item->data;

		if (pScene->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	if (fade) {
		float fadeRatio = (float)frameCount / (float)maxFadeFrames;
		// Render the black square with alpha on the screen
		SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
		SDL_RenderFillRect(app->render->renderer, &screenRect);
	}

	return ret;
}

bool SceneManager::CleanUp()
{
	return true;
}

bool SceneManager::LoadState(pugi::xml_node node)
{
	pugi::xml_node SceneNode = node.first_child();

	Character* aux;

	switch (SceneNode.attribute("currentScene").as_int())
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		for (size_t i = 0; i < scenes.Count(); i++)
		{
			if (scenes[i]->sceneType == SceneType::LEVEL1) {
				SceneNode = SceneNode.child("player");
				if (currentScene->sceneType == SceneType::LEVEL1) {
					for (size_t i = 0; i < currentScene->players.size(); i++)
					{
						aux = (Character*)currentScene->players[i];
						aux->TpToCell(SceneNode.attribute("x").as_int(), SceneNode.attribute("y").as_int());
						SceneNode = SceneNode.next_sibling("player");
					}
				}
			}
		}
		break;
	case 3:
		for (size_t i = 0; i < scenes.Count(); i++)
		{
			if (scenes[i]->sceneType == SceneType::LEVEL2) {
				SceneNode = SceneNode.child("player");
				if (currentScene->sceneType == SceneType::LEVEL2) {
					for (size_t i = 0; i < currentScene->players.size(); i++)
					{
						aux = (Character*)currentScene->players[i];
						aux->TpToCell(SceneNode.attribute("x").as_int(), SceneNode.attribute("y").as_int());
						SceneNode = SceneNode.next_sibling("player");
					}
				}
			}
		}
		break;
	case 4:
		break;
	default:
		break;
	}

	return true;
}

void SceneManager::ChangeScane(Scene* newScene)
{
	fade = true;
	previousScene = currentScene;
	this->newScene = newScene;
	currentStep = Fade_Step::TO_BLACK;
}

void SceneManager::MakeFade()
{
	if (currentStep == Fade_Step::TO_BLACK)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			if (currentScene != nullptr) 
			{
				currentScene->CleanUp();

				currentScene->active = false;
			}

			currentScene = newScene;

			if (currentScene->sceneType == MENU ||
				currentScene->sceneType == WIN_SCREEN ||
				currentScene->sceneType == LOSE_SCREEN) {
				app->map->CleanUp();
				app->map->active = false;
				app->entityManager->CleanUp();
				CleanAllLevels();
			}

			currentScene->Init();
			currentScene->Start();
			currentStep = Fade_Step::FROM_BLACK;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_Step::NO;
			fade = false;
		}
	}
}

void SceneManager::OpenSettings()
{
	currentScene->settings = true;
	settings->Start();
	settings->active = true;
}

void SceneManager::CloseSettings()
{
	settings->CleanUp();
	settings->active = false;
	if (currentScene == menu) {
		app->guiManager->ActiveAllGui();
		currentScene->settings = false;
	}
	else {
		OpenGamePause();
	}
}

bool SceneManager::SaveState(pugi::xml_node node)
{
	bool ret = true;

	pugi::xml_node scene = node.append_child(currentScene->name.GetString());
	scene.append_attribute("currentScene").set_value(currentScene->sceneType);

	for (size_t i = 0; i < currentScene->players.size(); i++)
	{
		pugi::xml_node player = scene.append_child("player");
		Character* aux = dynamic_cast<Character*>(currentScene->players[i]);
		player.append_attribute("x").set_value(aux->GetTile().x);
		player.append_attribute("y").set_value(aux->GetTile().y);
	}

	return ret;
}

void SceneManager::OpenGamePause()
{
	currentScene->settings = true;
	gamePause->Start();
	gamePause->active = true;
}

void SceneManager::CloseGamePause()
{
	gamePause->CleanUp();
	gamePause->active = false;
	currentScene->settings = false;
}

void SceneManager::OpenInventory()
{
	currentScene->inventory = true;
	inventoryMenu->Start();
	inventoryMenu->active = true;
}

void SceneManager::CloseInventory()
{
	inventoryMenu->CleanUp();
	inventoryMenu->active = false;
	currentScene->inventory = false;
}

void SceneManager::CleanAllLevels() {
	level1->CleanUp();
	level2->CleanUp();
}

Scene* SceneManager::GetSceneFromID(SceneType sceneType)
{
	Scene* auxScene = nullptr;

	switch (sceneType)
	{
	case INTRO:
		auxScene = intro;
		break;
	case GAME_TITLE:
		auxScene = gameTitle;
	case MENU:
		auxScene = menu;
		break;
	case LEVEL1:
		auxScene = level1;
		break;
	case LEVEL2:
		auxScene = level2;
		break;
	case LEVEL3:
		auxScene = level3;
	case LEVEL4:
		auxScene = level4;
	case NONE:
		break;
	default:
		break;
	}

	return auxScene;
}
