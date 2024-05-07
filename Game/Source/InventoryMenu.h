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

	bool OnGuiMouseClickEvent(GuiControl* control);

	uint windowW, windowH;

private:

	SDL_Texture* inventoryTexture = nullptr;

	uint textuerW = 0;
	uint textuerH = 0;

	SDL_Rect quat;
	int X, Y;

	SDL_Texture* characterTexture = nullptr;	//Character owner of the inventary
	SDL_Texture* itemTexture = nullptr;			//All other items
};	

#endif //__INVENTORY_MENU__