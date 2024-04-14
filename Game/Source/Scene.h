#ifndef __SCENE_H__
#define __SCENE_H__

#include "SString.h"
#include <vector>

using namespace std;

#include "PugiXml/src/pugixml.hpp"

class Entity;
class Player;
class Enemy;
class NPC;
class GuiControl;

enum SceneType
{
	INTRO,
	MENU,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	NONE
};

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

	Player* GetPlayer() {
		return (Player*)players.front();
	}

	Enemy* GetEnemy(uint id) {
		return (Enemy*)enemies.at(id);
	}

	Player* GetZhaak() {
		return (Player*)zhaak;
	}

	Player* GetEli() {
		return (Player*)eli;
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

	virtual void LockCamera() {

	};

	void DeleteEnemy(Enemy* enemy) {
		for (size_t i = 0; i < enemies.size(); i++)
		{
			if (enemies[i] == (Entity*)enemy) {
				enemies[i] = nullptr;
				enemies.erase(enemies.begin() + i);
			}
		}
	}


public:

	pugi::xml_node sceneconfig;
	pugi::xml_node sceneload;
	uint windowW, windowH;

	SString name;
	bool active;
	bool settings = false;
	Player* zhaak = nullptr;
	Player* eli = nullptr;
	bool quit = false;

	vector<Entity*> enemies;
	vector<Entity*> players;
	vector<Entity*> npcs;

	Entity* cameraFocus = nullptr;

	SceneType sceneType;

	bool loaded = false;
};

#endif // __SCENE_H__
