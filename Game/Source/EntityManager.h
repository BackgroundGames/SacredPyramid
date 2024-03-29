#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class Character;
class Player;
class Enemy;

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

	void DestroyEntity(Character* entity);

	void AddEntity(Character* entity);

	int NextTurn();

public:

	List<Enemy*> enemies;
	List<Player*> players;
	List<Character*> CombatList;
	Character* currentCharacterTurn;

	int turn = 0;
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
	Entity* CreateEntity(EntityType type, PlayerType p_type = PlayerType::UNKNOWN, EnemyType e_type = EnemyType::UNKNOWN, ItemType i_type = ItemType::UNKNOWN);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	void StartCombat(List<Entity*> enemies);

public:

	List<Entity*> entities;
	CombatManager* combatManager = nullptr;
};

#endif // __ENTITYMANAGER_H__
