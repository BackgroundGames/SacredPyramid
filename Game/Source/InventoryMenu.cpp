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

	app->tex->GetSize(inventoryTexture, textuerW, textuerH);

	weaponRect = { 407, 1901, 76, 76 };
	armorRect = { 407, 2004, 76, 76 };
	accessoryRect = { 407, 2104, 76, 76 };
	consumableRect = { 407, 2204, 76, 76 };

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

	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);
	MouseClickEvent(mouseX, mouseY);

	X = windowW / 2 - app->render->camera.x - textuerW / 2;
	Y = windowH / 2 - app->render->camera.y - textuerH / 2;
	app->render->DrawTexture(inventoryTexture, X, Y);
	app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->texture, X + 200, Y + 250, &app->sceneManager->currentScene->GetPlayer()->currentAnimation->GetCurrentFrame());

	X += 420; Y += 140;
	//Weapon Slot
	if (!app->sceneManager->currentScene->GetPlayer()->inventory.weapon.isDropped)
		app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->inventory.weapon.texture, X, Y);
	if (app->debug)
		app->render->DrawRectangle(weaponRect, 255, 0, 0, 255, false);

	Y += 100;
	//Armor Slot
	if (!app->sceneManager->currentScene->GetPlayer()->inventory.weapon.isDropped)
		app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->inventory.weapon.texture, X, Y);
	if (app->debug)
		app->render->DrawRectangle(armorRect, 255, 0, 0, 255, false);

	Y += 100;
	//Accessory Slot
	if (!app->sceneManager->currentScene->GetPlayer()->inventory.accessory.isDropped)
		app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->inventory.accessory.texture, X, Y);
	if (app->debug)
		app->render->DrawRectangle(accessoryRect, 255, 0, 0, 255, false);

	Y += 100;
	//Consumable Slot
	if (!app->sceneManager->currentScene->GetPlayer()->inventory.consumable.isDropped)
		app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->inventory.consumable.texture, X, Y);
	if (app->debug)
		app->render->DrawRectangle(consumableRect, 255, 0, 0, 255, false);

	X += 246; Y -= 455; 
	//Non-Eqipped Slots
	for (size_t i = 0; i < app->sceneManager->currentScene->GetPlayer()->inventory.invenotryItem.size(); i++)
	{
		if (app->sceneManager->currentScene->GetPlayer()->inventory.invenotryItem.at(i) != nullptr)
		{
			Y += 100;
			if (app->sceneManager->currentScene->GetPlayer()->inventory.invenotryItem.at(i)->texture != nullptr)
				app->render->DrawTexture(app->sceneManager->currentScene->GetPlayer()->inventory.invenotryItem.at(i)->texture, X, Y);
			
			SDL_Rect aux = { X - 20, Y - 23, 76, 76 };
			non_equipped.push_back(aux);

			if (app->debug)
				app->render->DrawRectangle(non_equipped.at(i), 255, 0, 0, 255, false);
		}

		if (i == 4)
		{
			X += 104;
			Y -= 500;
		}
	}

	app->render->DrawText("health:", 300, 120, 130, 20, 255, 255, 255, 0);
	statsText = std::to_string(app->sceneManager->currentScene->GetPlayer()->stats.health);
	const char* text = statsText.c_str();
	app->render->DrawText(text, 440, 120, 20, 20, 255, 255, 255, 0);

	app->render->DrawText("damge:", 300, 145, 110, 20, 255, 255, 255, 0);
	statsText = std::to_string(app->sceneManager->currentScene->GetPlayer()->inventory.weapon.damage);
	text = statsText.c_str();
	app->render->DrawText(text, 440, 145, 20, 20, 255, 255, 255, 0);

	app->render->DrawText("mvp:", 300, 170, 70, 20, 255, 255, 255, 0);
	statsText = std::to_string(app->sceneManager->currentScene->GetPlayer()->stats.movement);
	text = statsText.c_str();
	app->render->DrawText(text, 440, 170, 20, 20, 255, 255, 255, 0);

	return true;
}

bool InventoryMenu::CleanUp()
{
	app->tex->UnLoad(inventoryTexture);
	return true;
}

bool InventoryMenu::MouseClickEvent(int mouseX, int mouseY)
{
	//Left Button to Unequip an Item
	if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN)
	{
		if (mouseX > (weaponRect.x + app->render->camera.x) && mouseY > (weaponRect.y + app->render->camera.y) && mouseX < (weaponRect.x + app->render->camera.x + weaponRect.w) && mouseY < (weaponRect.y + app->render->camera.y + weaponRect.h))
		{
			//Weapon aux = app->sceneManager->currentScene->GetPlayer()->inventory.weapon;
			//app->sceneManager->currentScene->GetPlayer()->inventory.invenotryItem.push_back(&aux);
		}

		if (mouseX > (armorRect.x + app->render->camera.x) && mouseY > (armorRect.y + app->render->camera.y) && mouseX < (armorRect.x + app->render->camera.x + armorRect.w) && mouseY < (armorRect.y + app->render->camera.y + armorRect.h))
		{
			//Armor aux = app->sceneManager->currentScene->GetPlayer()->inventory.armor;
			//app->sceneManager->currentScene->GetPlayer()->inventory.invenotryItem.push_back(&aux);
		}

		if (mouseX > (accessoryRect.x + app->render->camera.x) && mouseY > (accessoryRect.y + app->render->camera.y) && mouseX < (accessoryRect.x + app->render->camera.x + accessoryRect.w) && mouseY < (accessoryRect.y + app->render->camera.y + accessoryRect.h))
		{
			//Accessory aux = app->sceneManager->currentScene->GetPlayer()->inventory.accessory;
			//app->sceneManager->currentScene->GetPlayer()->inventory.invenotryItem.push_back(&aux);
		}

		if (mouseX > (consumableRect.x + app->render->camera.x) && mouseY > (consumableRect.y + app->render->camera.y) && mouseX < (consumableRect.x + app->render->camera.x + consumableRect.w) && mouseY < (consumableRect.y + app->render->camera.y + consumableRect.h))
		{
			//Consumable aux = app->sceneManager->currentScene->GetPlayer()->inventory.consumable;
			//app->sceneManager->currentScene->GetPlayer()->inventory.invenotryItem.push_back(&aux);
		}
	}
	
	//Keyboard button D to drop an Item
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
	{
		if (mouseX > (weaponRect.x + app->render->camera.x) && mouseY > (weaponRect.y + app->render->camera.y) && mouseX < (weaponRect.x + app->render->camera.x + weaponRect.w) && mouseY < (weaponRect.y + app->render->camera.y + weaponRect.h))
		{
			Weapon aux = app->sceneManager->currentScene->GetPlayer()->inventory.weapon;
			app->sceneManager->currentScene->GetPlayer()->inventory.weapon.isDropped = true;
			DropItem(1);
		}

		if (mouseX > (armorRect.x + app->render->camera.x) && mouseY > (armorRect.y + app->render->camera.y) && mouseX < (armorRect.x + app->render->camera.x + armorRect.w) && mouseY < (armorRect.y + app->render->camera.y + armorRect.h))
		{
			Armor aux = app->sceneManager->currentScene->GetPlayer()->inventory.armor;
			app->sceneManager->currentScene->GetPlayer()->inventory.armor.isDropped = true;
			DropItem(2);
		}

		if (mouseX > (accessoryRect.x + app->render->camera.x) && mouseY > (accessoryRect.y + app->render->camera.y) && mouseX < (accessoryRect.x + app->render->camera.x + accessoryRect.w) && mouseY < (accessoryRect.y + app->render->camera.y + accessoryRect.h))
		{
			Accessory aux = app->sceneManager->currentScene->GetPlayer()->inventory.accessory;
			app->sceneManager->currentScene->GetPlayer()->inventory.accessory.isDropped = true;
			DropItem(3);
		}

		if (mouseX > (consumableRect.x + app->render->camera.x) && mouseY > (consumableRect.y + app->render->camera.y) && mouseX < (consumableRect.x + app->render->camera.x + consumableRect.w) && mouseY < (consumableRect.y + app->render->camera.y + consumableRect.h))
		{
			Consumable aux = app->sceneManager->currentScene->GetPlayer()->inventory.consumable;
			app->sceneManager->currentScene->GetPlayer()->inventory.consumable.isDropped = true;
			DropItem(4);
		}
	}

	return true;
}

void InventoryMenu::DropItem(uint item_type)
{
	Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, PlayerType::UNKNOWN, EnemyType::UNKNOWN, item_type);
	item->id = app->sceneManager->currentScene->GetPlayer()->inventory.consumable.id;
	app->sceneManager->currentScene->items.push_back(item);
	item->parameters = app->sceneManager->currentScene->GetPlayer()->inventory.consumable.parameters;
	item->Start();
}
