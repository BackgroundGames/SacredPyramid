#ifndef __SCENE_H__
#define __SCENE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class Player;
class Enemy;
class GuiControl;

class Scene
{
public:

	Scene() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node config)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	Player* GetPlayer() { return player; }

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}


public:

	pugi::xml_node sceneconfig;
	uint windowW, windowH;

	SString name;
	bool active;
	bool settings = false;
	Player* player = nullptr;
	bool quit = false;
};

#endif // __SCENE_H__
