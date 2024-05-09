#include "WinScreen.h"
#include "App.h"
#include "Textures.h"
#include "Window.h"
#include "Audio.h"

WinScreen::WinScreen()
{
	name.Create("win_screen");
}

WinScreen::~WinScreen()
{
}

bool WinScreen::Awake(pugi::xml_node config)
{
	sceneconfig = config;
	return true;
}

bool WinScreen::Start()
{
	img = app->tex->Load(sceneconfig.attribute("texturePath").as_string());
	app->tex->GetSize(img, texW, texH);
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	app->win->GetWindowSize(windowW, windowH);
	winningFx = app->audio->LoadFx(sceneconfig.attribute("audio").as_string());
	app->audio->PlayFx(winningFx);
	return true;
}

bool WinScreen::PreUpdate()
{
	return true;
}

bool WinScreen::Update(float dt)
{
	return true;
}

bool WinScreen::PostUpdate()
{
	app->render->DrawTexture(img, windowW / 2 - texW / 2, windowH / 2 - texH / 2, NULL);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->menu);

	return true;
}

bool WinScreen::CleanUp()
{
	app->tex->UnLoad(img);
	app->audio->UnloadFx(winningFx);
	return true;
}
