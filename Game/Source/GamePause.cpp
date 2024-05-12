#include "GamePause.h"
#include "Window.h"
#include "Input.h"
#include "SceneManager.h"
#include "GuiManager.h"
#include "Audio.h"
#include "Player.h"

GamePause::GamePause() : Scene()
{
	name.Create("gamepause");
}

GamePause::~GamePause()
{
}

bool GamePause::Awake(pugi::xml_node config)
{
	sceneconfig = config;
	return true;
}

bool GamePause::Start()
{
	app->win->GetWindowSize(windowW, windowH);
	quat.w = windowW;
	quat.h = windowH;
	quat.x = 0;
	quat.y = 0;

	SDL_Rect resumePos = { windowW / 2 - 75, windowH / 2 - 25 - (windowH / 7) * 2, 150,50 };
	resumeButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "RESUME", resumePos, this);
	SDL_Rect settingsPos = { windowW / 2 - 100, windowH / 2 - 25 - (windowH / 7), 200,50 };
	settingsButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "SETTINGS", settingsPos, this);
	SDL_Rect menuPos = { windowW / 2 - 50, windowH / 2 - 25, 100,50 };
	menuButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "MENU", menuPos, this);
	SDL_Rect exitPos = { windowW / 2 - 50, windowH / 2 - 25 + (windowH / 7), 100,50 };
	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "EXIT", exitPos, this);

	//app->audio->PlayMusic(sceneconfig.attribute("audio").as_string(), 0);

	return true;
}

bool GamePause::PreUpdate()
{
	return true;
}

bool GamePause::Update(float dt)
{
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(125));
	SDL_RenderFillRect(app->render->renderer, &quat);

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
		app->sceneManager->currentScene->GetPlayer()->exploringState = app->sceneManager->currentScene->GetPlayer()->previousState;
		//app->audio->PlayMusic(app->sceneManager->currentScene->sceneconfig.attribute("audio").as_string(), 0);
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
