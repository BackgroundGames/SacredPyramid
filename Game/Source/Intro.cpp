#include "Intro.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "SceneManager.h"
#include "Window.h"
#include "Audio.h"

Intro::Intro() : Scene()
{
	name.Create("intro");
}

Intro::~Intro()
{
}

bool Intro::Awake(pugi::xml_node config)
{
	sceneconfig = config;

	animation.speed = 0.05f;
	animation.loop = false;
	animation.PushBack({0,0,1280,720});
	animation.PushBack({ 1280,0,1280,720 });
	animation.PushBack({ 1280,720,1280,720 });
	return true;
}

bool Intro::Start()
{
	img = app->tex->Load(sceneconfig.attribute("texturePath").as_string());
	app->tex->GetSize(img, texW, texH);
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->win->GetWindowSize(windowW, windowH);
	introFx = app->audio->LoadFx(sceneconfig.attribute("audio").as_string());
	app->audio->PlayFx(introFx);
	return true;
}

bool Intro::PreUpdate()
{
	return true;
}

bool Intro::Update(float dt)
{
	animation.Update();
	return true;
}

bool Intro::PostUpdate()
{
	//OPTICK_EVENT();

	app->render->DrawTexture(img, 0, 0, &animation.GetCurrentFrame());

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->gameTitle);

	//Debug key to skip the Menu
	if (app->input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->level1);

	
	return true;
}

bool Intro::CleanUp()
{
	app->tex->UnLoad(img);
	app->audio->UnloadFx(introFx);
	return true;
}
