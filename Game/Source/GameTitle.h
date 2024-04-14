#ifndef __GAME_TITLE__
#define __GAME_TITLE__

#include "Scene.h"

struct SDL_Texture;

class GameTitle : public Scene
{
public:

	GameTitle();

	// Destructor
	virtual ~GameTitle();

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
	int fx = -1;
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	int introFx;
};

#endif //__GAME_TITLE__