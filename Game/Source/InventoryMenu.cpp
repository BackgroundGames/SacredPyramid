#include "InventoryMenu.h"
#include "Window.h"
#include "Textures.h"

InventoryMenu::InventoryMenu()
{
	name.Create("inventory");
}

InventoryMenu::~InventoryMenu()
{
}

bool InventoryMenu::Awake(pugi::xml_node config)
{
	return true;
}

bool InventoryMenu::Start()
{
	app->win->GetWindowSize(windowW, windowH);
	quat.w = windowW;
	quat.h = windowH;
	quat.x = 0;
	quat.y = 0;

	texture = app->tex->Load("Assets/Textures/inventory_placeholder.png");

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

	app->render->DrawTexture(texture, windowW / 2, windowH / 2);

	return true;
}

bool InventoryMenu::PostUpdate()
{
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
