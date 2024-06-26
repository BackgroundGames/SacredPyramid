#ifndef __MENU__
#define __MENU__
#include "Scene.h"

#include "GuiControlButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"
#include "GuiControl.h"

struct SDL_Texture;

class Menu :  public Scene
{
public:

	Menu();

	// Destructor
	virtual ~Menu();

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
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	pugi::xml_node config;

	// L15: TODO 2: Declare a GUI Control Button 
	GuiControlButton* startButton;
	GuiControlButton* continueButton;
	GuiControlButton* settingsButton;
	GuiControlButton* creditsButton;
	GuiControlButton* exitButton;
};

#endif //__MENU__