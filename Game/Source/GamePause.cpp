#include "GamePause.h"
#include "Window.h"
#include "Input.h"
#include "SceneManager.h"
#include "GuiManager.h"
#include "Audio.h"

GamePause::GamePause() : Scene()
{
	name.Create("gamepause");
}

GamePause::~GamePause()
{
}

bool GamePause::Awake(pugi::xml_node config)
{
	return true;
}

bool GamePause::Start()
{
	app->win->GetWindowSize(windowW, windowH);

	SDL_Rect resumePos = { windowW / 2 - 75, windowH / 2 - 25 - (windowH / 7) * 2, 150,50 };
	resumeButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "RESUME", resumePos, this);
	SDL_Rect settingsPos = { windowW / 2 - 100, windowH / 2 - 25 - (windowH / 7), 200,50 };
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "SETTINGS", settingsPos, this);
	SDL_Rect menuPos = { windowW / 2 - 50, windowH / 2 - 25, 100,50 };
	menuButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "MENU", menuPos, this);
	SDL_Rect exitPos = { windowW / 2 - 50, windowH / 2 - 25 + (windowH / 7), 100,50 };
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "EXIT", exitPos, this);

	return true;
}

bool GamePause::PreUpdate()
{
	return true;
}

bool GamePause::Update(float dt)
{
	return true;
}

bool GamePause::PostUpdate()
{
	return true;
}

bool GamePause::CleanUp()
{
	app->guiManager->DeleteGuiControl(resumeButton);
	app->guiManager->DeleteGuiControl(settingsButton);
	app->guiManager->DeleteGuiControl(menuButton);
	app->guiManager->DeleteGuiControl(exitButton);

	return true;
}

bool GamePause::OnGuiMouseClickEvent(GuiControl* control)
{
	if (control->id == 10) {
		app->sceneManager->CloseGamePause();
	}
	if (control->id == 11) {
		app->sceneManager->OpenSettings();
		app->sceneManager->CloseGamePause();
	}
	if (control->id == 12) {
		app->sceneManager->CloseGamePause();
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->menu);
	}
	if (control->id == 13) {
		quit = true;
	}
	return true;
}