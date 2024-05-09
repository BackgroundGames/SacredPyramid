#ifndef __WIN_SCREEN__
#define __WIN_SCREEN__

#include "Scene.h"

struct SDL_Texture;

class WinScreen : public Scene
{
public:

	WinScreen();

	// Destructor
	virtual ~WinScreen();

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
	int winningFx;
};

#endif //__WIN_SCREEN__