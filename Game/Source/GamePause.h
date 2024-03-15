#ifndef __GAMEPAUSE__
#define __GAMEPAUSE__

#include "Scene.h"
#include "GuiControl.h"
#include "GuiControlButton.h"

class GamePause :  public Scene
{
public:

	GamePause();

	// Destructor
	virtual ~GamePause();

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
	GuiControlButton* resumeButton;
	GuiControlButton* settingsButton;
	GuiControlButton* menuButton;
	GuiControlButton* exitButton;
};

#endif //__SETTINGS__