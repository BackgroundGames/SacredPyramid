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
	return true;
}

bool Intro::Start()
{
	img = app->tex->Load(sceneconfig.attribute("texturePath").as_string());
	app->tex->GetSize(img, texW, texH);
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->win->GetWindowSize(windowW, windowH);
	return true;
}

bool Intro::PreUpdate()
{
	return true;
}

bool Intro::Update(float dt)
{
	return true;
}

bool Intro::PostUpdate()
{
	//OPTICK_EVENT();

	app->render->DrawTexture(img, windowW/2 - texW/2, windowH / 2 - texH / 2,NULL);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->menu);

	//Debug key to skip the Menu
	if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->level1);

	
	return true;
}

bool Intro::CleanUp()
{
	app->tex->UnLoad(img);
	return true;
}
