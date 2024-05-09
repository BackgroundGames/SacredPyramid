#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include <vector>
#include "List.h"
#include "Pathfinding.h"
#include "Weapon.h"

using namespace std;

class GuiControlButton;
class Character;
class Player;
class Enemy;

enum Fade_StepFade
{
	NOF,
	TO_BLACKF,
	FROM_BLACKF
};

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

	void DestroyEntity(Character* entity, int i);

	int NextTurn();

	void EndCombat();

	void CheckIfHit(iPoint& dest, Weapon* weapon);

	Player* GetClosestPlayer(Character* entity, int& dist);

	void CheckIfCharDead();

	void UIEvent(int id);

	void PreapareUINextTurn();

	void UpdateNavigation(Entity* pFocus = nullptr);

	void ClearNavigation();

public:

	Enemy* summoner = nullptr;
	List<Enemy*> enemies;
	vector<Player*> players;
	vector<Character*> CombatList;

	Character* currentCharacterTurn = nullptr;
	int turn = 0;
	Uint32 startTime;
	vector<int> pathfindingBlock;
	float seconds = 0.0f;

	int playersAlive;

	GuiControlButton* nextTurnButton;
	GuiControlButton* attackButton;
	GuiControlButton* moveButton;

	SDL_Texture* turnArrow = nullptr;
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

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type, PlayerType p_type = PlayerType::UNKNOWN, EnemyType e_type = EnemyType::UNKNOWN, ItemType i_type = ItemType::UNKNOWN);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

	void StartCombat(vector<Entity*> &enemies, Entity* summoner);

	void MakeStartCombatFade();
	
	void MakeEndCombatFade();

public:

	List<Entity*> entities;
	CombatManager* combatManager = nullptr;
	bool inCombat = false;
	bool startCombat = false;
	bool combatFinished = false;
	bool hasLosed = false;

	uint windowW, windowH;
	SDL_Rect screenRect;
	Fade_StepFade currentStep = Fade_StepFade::NOF;
	Uint32 maxFadeFrames = 75;
	Uint32 frameCount = 0;

	pugi::xml_node config;
};

#endif // __ENTITYMANAGER_H__
