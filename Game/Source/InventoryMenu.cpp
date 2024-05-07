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
	characterTexture = app->tex->Load(sceneconfig.attribute("texturePath").as_string());

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

	return true;
}

bool InventoryMenu::PostUpdate()
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->sceneManager->CloseInventory();
		app->sceneManager->currentScene->GetPlayer()->exploringState = app->sceneManager->currentScene->GetPlayer()->previousState;
	}

	X = windowW / 2 - app->render->camera.x - textuerW / 2;
	Y = windowH / 2 - app->render->camera.y - textuerH / 2;
	app->render->DrawTexture(inventoryTexture, X, Y);
	app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->texture, X + 200, Y + 250, &app->sceneManager->currentScene->GetPlayer()->currentAnimation->GetCurrentFrame());

	X += 420; Y += 140;
	app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->inventory.weapon.texture, X, Y);
	app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->inventory.weapon.texture, X, Y += 100);
	app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->inventory.accessory.texture, X, Y += 100);
	app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->inventory.consumable.texture, X, Y += 100);

	/*X += 246; Y -= 455;
	for (size_t i = 0; i < 10; i++)
	{
		app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->inventory.weapon.texture, X, Y += 100);
		if (i == 4)
		{
			X += 104;
			Y -= 500;
		}
	}*/

	return true;
}

bool InventoryMenu::CleanUp()
{
	return true;
}

bool InventoryMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	
	return true;
}
