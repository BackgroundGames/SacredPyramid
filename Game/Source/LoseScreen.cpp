#include "LoseScreen.h"
#include "App.h"
#include "Textures.h"
#include "Window.h"
#include "Audio.h"
#include "GuiManager.h"
#include "QuestManager.h"

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
	app->audio->PlayMusic(sceneconfig.attribute("audio").as_string(),0);

	SDL_Rect continuePos = { 296, 530, 200,50 };
	continueButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, "continue", continuePos, this);
	SDL_Rect menuPos = { 837, 530, 100,50 };
	menuButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "menu", menuPos, this);

	app->questManager->CleanUp();

	return true;
}

bool LoseScreen::PreUpdate()
{
	return true;
}

bool LoseScreen::Update(float dt)
{
	app->render->DrawTexture(img, windowW / 2 - texW / 2, windowH / 2 - texH / 2, NULL);
	return true;
}

bool LoseScreen::PostUpdate()
{
	return true;
}

bool LoseScreen::CleanUp()
{
	app->tex->UnLoad(img);
	app->audio->UnloadFx(loseingFx);
	app->guiManager->DeleteGuiControl(continueButton);
	app->guiManager->DeleteGuiControl(menuButton);
	app->guiManager->CleanUp();
	return true;
}

bool LoseScreen::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	if (control->id == 0) {
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->previousScene);
	}
	if (control->id == 1) {
		app->sceneManager->ChangeScane((Scene*)app->sceneManager->menu);
	}

	return true;
}