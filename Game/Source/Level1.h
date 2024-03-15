#ifndef __LEVEL1_H__
#define __LEVEL1_H__

#include "Scene.h"
#include "Player.h"
#include "Enemy.h"

struct SDL_Texture;

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

	Player* GetPLayer() {	return player;	}

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	SDL_Texture* mouseTileTex = nullptr;
};

#endif // __LEVEL1_H__
