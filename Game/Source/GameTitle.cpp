#include "GameTitle.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "SceneManager.h"
#include "Window.h"
#include "Audio.h"

GameTitle::GameTitle() : Scene()
{
	name.Create("gameTitle");
}

GameTitle::~GameTitle()
{
}

bool GameTitle::Awake(pugi::xml_node config)
{
	sceneconfig = config;
	return true;
}

bool GameTitle::Start()
{
	img = app->tex->Load(sceneconfig.attribute("texturePath").as_string());
	app->tex->GetSize(img, texW, texH);
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->win->GetWindowSize(windowW, windowH);
	introFx = app->audio->LoadFx("Assets/Audio/Fx/logo.wav");
	app->audio->PlayFx(introFx);
	return true;
}

bool GameTitle::PreUpdate()
{
	return true;
}

bool GameTitle::Update(float dt)
{
	return true;
}

bool GameTitle::PostUpdate()
{
	//OPTICK_EVENT();

	app->render->DrawTexture(img, windowW / 2 - texW / 2, windowH / 2 - texH / 2, NULL);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->menu);

	//Debug key to skip the Menu
	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->level1);


	return true;
}

bool GameTitle::CleanUp()
{
	app->tex->UnLoad(img);
	app->audio->UnloadFx(introFx);
	return true;
}
