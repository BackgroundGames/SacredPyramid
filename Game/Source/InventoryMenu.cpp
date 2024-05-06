#include "InventoryMenu.h"
#include "App.h"
#include "Window.h"
#include "Textures.h"
#include "GuiManager.h"
#include "SceneManager.h"
#include "Player.h"

InventoryMenu::InventoryMenu()
{
	name.Create("inventory");
}

InventoryMenu::~InventoryMenu()
{
}

bool InventoryMenu::Awake(pugi::xml_node config)
{
	sceneconfig = config;
	return true;
}

bool InventoryMenu::Start()
{
	app->win->GetWindowSize(windowW, windowH);
	quat.w = windowW;
	quat.h = windowH;
	quat.x = 0;
	quat.y = 0;

	inventoryTexture = app->tex->Load(sceneconfig.attribute("texturePath").as_string());
	exitTexture = app->tex->Load(sceneconfig.attribute("exitButton").as_string());

	app->tex->GetSize(exitTexture, textuerW, textuerH);

	SDL_Rect exitPos = {0,  0, 42,42 };

	exitButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "hola", exitPos, this, { 0,0,(int)textuerW,(int)textuerH }, exitTexture);

	app->tex->GetSize(inventoryTexture, textuerW, textuerH);

	return true;
}

bool InventoryMenu::PreUpdate()
{
	return true;
}

bool InventoryMenu::Update(float dt)
{
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(125));
	SDL_RenderFillRect(app->render->renderer, &quat);

	app->render->DrawTexture(inventoryTexture, windowW / 2 - app->render->camera.x - textuerW/2, windowH / 2 - app->render->camera.y - textuerH/2);

	return true;
}

bool InventoryMenu::PostUpdate()
{
	return true;
}

bool InventoryMenu::CleanUp()
{
	app->guiManager->DeleteGuiControl(exitButton);

	return true;
}

bool InventoryMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	if (control->id == 14) {
		app->sceneManager->CloseInventory();
		app->sceneManager->currentScene->GetPlayer()->exploringState = app->sceneManager->currentScene->GetPlayer()->previousEState;
	}

	return true;
}
