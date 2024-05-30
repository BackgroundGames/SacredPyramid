#ifndef __INVENTORY_MENU__
#define __INVENTORY_MENU__

#include "Scene.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

class InventoryMenu : public Scene
{
public:

	InventoryMenu();

	// Destructor
	virtual ~InventoryMenu();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool MouseClickEvent(int mouseX, int mouseY);

	uint windowW, windowH;

private:

	SDL_Texture* inventoryTexture = nullptr;

	uint textuerW = 0;
	uint textuerH = 0;

	SDL_Rect quat;
	int X, Y;


	std::string statsText;

	//debug
	bool bol = true;
	SDL_Rect weaponRect;							//Rectangle of Weapon slot
	SDL_Rect armorRect;								//Rectangle of Armor slot
	SDL_Rect accessoryRect;							//Rectangle of Accessory slot
	SDL_Rect consumableRect;						//Rectangle of Consumable slot

	std::vector<SDL_Rect> non_equipped;
};	

#endif //__INVENTORY_MENU__