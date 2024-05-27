#ifndef __LEVEL1_H__
#define __LEVEL1_H__

#include "Scene.h"
#include "Player.h"
#include "Zhaak.h"
#include "Eli.h"
#include "Amir.h"
#include "Enemy.h"
#include "NPC.h"
#include "GuiControl.h"

struct SDL_Texture;
class Particle;

class Level1 : public Scene
{
public:

	Level1();

	// Destructor
	virtual ~Level1();

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

private:
	SDL_Texture* img;
	Particle* nextMap;
	float textPosX, textPosY = 0;
	uint texW, texH;
	SDL_Texture* mouseTileTex = nullptr;
	iPoint tabernTile = { 16,15 };
	iPoint puenteTile = { 37,11 };
};

#endif // __LEVEL1_H__
