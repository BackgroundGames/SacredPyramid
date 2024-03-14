#include "SceneManager.h"

#include "App.h"
#include "Window.h"
#include "GuiManager.h"
#include "Level1.h"
#include "SceneIntro.h"
#include "Menu.h"


SceneManager::SceneManager()
{
	name.Create("scenemanager");

	sceneIntro = new SceneIntro();
	menu = new Menu();
	level1 = new Level1();

	scenes.Add(sceneIntro);
	scenes.Add(menu);
	scenes.Add(level1);
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
	ChangeScane(sceneIntro);

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
	//settings->Start();
	//settings->active = true;
}

void SceneManager::CloseSettings()
{
	//settings->CleanUp();
	//settings->active = false;
	//app->guiManager->ActiveAllGui();
	//currentScene->settings = false;
}

bool SceneManager::SaveState(pugi::xml_node node)
{
	bool ret = true;

	pugi::xml_node scene = node.append_child(name.GetString());
	scene.append_attribute("currentScene").set_value(sceneType);

	return ret;
}
