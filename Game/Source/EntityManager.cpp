#include "EntityManager.h"
#include "Player.h"
#include "Zhaak.h"
#include "Eli.h"
#include "Enemy.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, uint id)
{
	Entity* entity = nullptr; 

	//Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:
		
		switch (id)
		{
		case 1:
			entity = new Zhaak();
			break;
		case 2:
			entity = new Eli();
			break;
		default:
			break;
		}
		break;

	case EntityType::ENEMY:
		entity = new Enemy();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

void EntityManager::StartCombat(List<Entity*> enemies)
{
	combatManager = new CombatManager();

	int length = 0;

	for (int i = 0; i < app->sceneManager->currentScene->players.Count(); i++)
	{
		combatManager->players.Add(app->sceneManager->currentScene->players[i]);
		dynamic_cast<Player*>(combatManager->players[i])->mainState = MainState::IN_COMBAT;
		length++;
	}

	for (int j = 0; j < enemies.Count(); j++)
	{
		combatManager->enemies.Add(enemies[j]);
		dynamic_cast<Enemy*>(combatManager->enemies[j])->mainState = MainState::IN_COMBAT;
		length++;
	}

	int paux = 0;
	int eaux = 0;

	for (size_t k = 0; k < length; k++)
	{
		if (k%2 == 0) {
			if (paux < combatManager->players.Count()) {
				combatManager->CombatList.Add(combatManager->players[paux]);
				paux++;
			}
			else if (eaux < combatManager->enemies.Count()) {
				combatManager->CombatList.Add(combatManager->enemies[eaux]);
				eaux++;
			}
		}
		else {
			if (eaux < combatManager->enemies.Count()) {
				combatManager->CombatList.Add(combatManager->enemies[eaux]);
				eaux++;
			}
			else if (paux < combatManager->players.Count()) {
				combatManager->CombatList.Add(combatManager->players[paux]);
				paux++;
			}
		}
	}
}

bool EntityManager::Update(float dt)
{

	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	if (combatManager == nullptr) {

		for (item = entities.start; item != NULL && ret == true; item = item->next)
		{
			pEntity = item->data;

			if (pEntity->active == false) continue;
			ret = item->data->Update(dt);
		}

	}
	else {
		//ret = combatManager->Update();
	}

	return ret;
}

CombatManager::CombatManager()
{
}

CombatManager::~CombatManager()
{
}

bool CombatManager::Start()
{
	return false;
}

bool CombatManager::Update(float dt)
{

	Uint32 currentTime = SDL_GetTicks();
	Uint32 elapsedTime = currentTime - startTime;

	//Seconds = (elapsedTime / 1000) % 60;
	////Seconds = elapsedTime % 1000;
	//if (auxSeconds != Seconds) {
	//	MaxSeconds -= 1;
	//	auxSeconds = Seconds;
	//}

	return true;
}

bool CombatManager::CleanUp()
{
	return false;
}

void CombatManager::DestroyEntity(Entity* entity)
{
}

void CombatManager::AddEntity(Entity* entity)
{
}
