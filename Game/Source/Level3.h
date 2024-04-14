#ifndef __LEVEL3_H__
#define __LEVEL3_H__

#include "Scene.h"
#include "Player.h"
#include "Zhaak.h"
#include "Eli.h"
#include "Amir.h"
#include "Enemy.h"
#include "NPC.h"
#include "GuiControl.h"

struct SDL_Texture;

class Level3 : public Scene
{
public:

	Level3();

	// Destructor
	virtual ~Level3();

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

	void LockCamera();

	uint windowW, windowH;

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	SDL_Texture* mouseTileTex = nullptr;
	iPoint endTile = { 2,24 };
};

#endif // __LEVEL3_H__
