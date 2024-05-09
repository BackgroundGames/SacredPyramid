#ifndef __LOSE_SCREEN__
#define __LOSE_SCREEN__

#include "Scene.h"

struct SDL_Texture;

class LoseScreen : public Scene
{
public:

	LoseScreen();

	// Destructor
	virtual ~LoseScreen();

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

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	int loseingFx;
};

#endif //__LOSE_SCREEN__