#ifndef __LEVEL2_H__
#define __LEVEL2_H__

#include "Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "GuiControl.h"

struct SDL_Texture;

class Level2 :  public Scene
{
public:
	Level2();

	// Destructor
	virtual ~Level2();

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
	uint windowW, windowH;

	bool OnGuiMouseClickEvent(GuiControl* control);

	void LockCamera();

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	SDL_Texture* mouseTileTex = nullptr;
};

#endif