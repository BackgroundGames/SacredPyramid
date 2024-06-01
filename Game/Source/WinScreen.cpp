#include "WinScreen.h"
#include "App.h"
#include "Textures.h"
#include "Window.h"
#include "Audio.h"
#include "GuiManager.h"

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

	SDL_Rect menuPos = { 340, 530, 100,50 };
	menuButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, "menu", menuPos, this);
	SDL_Rect quitPos = { 837, 530, 100,50 };
	quitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "quit", quitPos, this);

	return true;
}

bool WinScreen::PreUpdate()
{
	return true;
}

bool WinScreen::Update(float dt)
{
	app->render->DrawTexture(img, windowW / 2 - texW / 2, windowH / 2 - texH / 2, NULL);
	return true;
}

bool WinScreen::PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN || app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->menu);

	if (quit)
		return false;

	return true;
}

bool WinScreen::CleanUp()
{
	app->tex->UnLoad(img);
	app->audio->UnloadFx(winningFx);
	app->guiManager->DeleteGuiControl(menuButton);
	app->guiManager->DeleteGuiControl(quitButton);
	app->guiManager->CleanUp();
	return true;
}

bool WinScreen::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	if (control->id == 0) {
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->menu);
	}
	if (control->id == 1) {
		quit = true;
	}

	return true;
}