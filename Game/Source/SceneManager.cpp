#include "SceneManager.h"

#include "App.h"
#include "Window.h"
#include "GuiManager.h"
#include "Settings.h"
#include "Level1.h"
#include "Level2.h"
#include "Intro.h"
#include "Menu.h"
#include "GamePause.h"
#include "Map.h"


SceneManager::SceneManager()
{
	name.Create("scenemanager");

	intro = new Intro();
	menu = new Menu();
	level1 = new Level1();
	level2 = new Level2();
	settings = new Settings();
	gamePause = new GamePause();

	scenes.Add(intro);
	scenes.Add(menu);
	scenes.Add(level1);
	scenes.Add(level2);
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
			if (currentScene != nullptr) {
				currentScene->CleanUp();
				currentScene->active = false;
			}

			currentScene = newScene;

			if (currentScene == menu) {
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

	pugi::xml_node scene = node.append_child(name.GetString());
	scene.append_attribute("currentScene").set_value(sceneType);

	return ret;
}

void SceneManager::CloseGamePause()
{
	gamePause->CleanUp();
	gamePause->active = false;
	currentScene->settings = false;
}

void SceneManager::OpenGamePause()
{
	currentScene->settings = true;
	gamePause->Start();
	gamePause->active = true;
}

void SceneManager::CleanAllLevels() {
	level1->CleanUp();
	level2->CleanUp();
}