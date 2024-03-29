#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class CombatManager {

public:

	CombatManager();

	// Destructor
	virtual ~CombatManager();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

public:

	List<Entity*> enemies;
	List<Entity*> players;
	List<Entity*> CombatList;

	Uint32 startTime;

};

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node config);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();


	// Additional methods
	Entity* CreateEntity(EntityType type, uint id);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	void StartCombat(List<Entity*> enemies);

public:

	List<Entity*> entities;
	CombatManager* combatManager = nullptr;
};

#endif // __ENTITYMANAGER_H__
