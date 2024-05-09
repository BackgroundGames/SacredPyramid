#include "LoseScreen.h"
#include "App.h"
#include "Textures.h"
#include "Window.h"
#include "Audio.h"

LoseScreen::LoseScreen()
{
	name.Create("lose_screen");
}

LoseScreen::~LoseScreen()
{
}

bool LoseScreen::Awake(pugi::xml_node config)
{
	sceneconfig = config;
	return true;
}

bool LoseScreen::Start()
{
	img = app->tex->Load(sceneconfig.attribute("texturePath").as_string());
	app->tex->GetSize(img, texW, texH);
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->win->GetWindowSize(windowW, windowH);
	loseingFx = app->audio->LoadFx(sceneconfig.attribute("audio").as_string());
	app->audio->PlayFx(loseingFx);
	return true;
}

bool LoseScreen::PreUpdate()
{
	return true;
}

bool LoseScreen::Update(float dt)
{
	return true;
}

bool LoseScreen::PostUpdate()
{
	app->render->DrawTexture(img, windowW / 2 - texW / 2, windowH / 2 - texH / 2, NULL);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->menu);
	
	return true;
}

bool LoseScreen::CleanUp()
{
	app->tex->UnLoad(img);
	app->audio->UnloadFx(loseingFx);
	return true;
}
