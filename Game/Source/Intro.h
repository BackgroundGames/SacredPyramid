#ifndef __SCENE_INTRO__
#define __SCENE_INTRO__

#include "Scene.h"
#include "Animation.h"

struct SDL_Texture;

class Intro : public Scene
{
public:

	Intro();

	// Destructor
	virtual ~Intro();

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
	uint texW, texH;
	int introFx;

	Animation animation;
};

#endif //__SCENE_INTRO__