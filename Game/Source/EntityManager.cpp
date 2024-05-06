#include "EntityManager.h"
#include "Window.h"
#include "Map.h"
#include "Audio.h"
#include "GuiManager.h"
#include "GuiControlButton.h"

#include "Player.h"
#include "Zhaak.h"
#include "Eli.h"
#include "Amir.h"

#include "Enemy.h"
#include "Bandit.h"
#include "Drunkard.h"
#include "EvilMummy.h"

#include "NPC.h"

#include "Item.h"
#include "Weapon.h"
#include "Armor.h"
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

	this->config = config;

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

	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

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

	app->win->GetWindowSize(windowW, windowH);

	screenRect = { 0, 0, (int)windowW, (int)windowH };

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

Entity* EntityManager::CreateEntity(EntityType type, PlayerType p_type, EnemyType e_type, ItemType i_type)
{
	Entity* entity = nullptr;

	Stats stats;
	Inventory inventory;

	//Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:

		switch (p_type)
		{
		case PlayerType::ZHAAK:
			stats.health = 3;
			inventory.weapon.damage = 1;
			inventory.weapon.range = 1;
			entity = new Zhaak(stats, inventory);
			break;

		case PlayerType::AMIR:
			entity = new Amir();
			break;

		case PlayerType::AMUMMY:
			break;

		case PlayerType::ELI:
			stats.health = 3;
			inventory.weapon.damage = 1;
			inventory.weapon.range = 3;
			entity = new Eli();
			break;

		case PlayerType::UNKNOWN:
			entity = new Player();
			break;

		default:
			break;
		}
		break;

	case EntityType::ENEMY:

		switch (e_type)
		{
		case EnemyType::BANDIT:
			stats.health = 1;
			inventory.weapon.range = 2;
			entity = new Bandit(stats, inventory);
			break;

		case EnemyType::DRUNKARD:
			inventory.weapon.range = 1;
			inventory.weapon.damage = 1;
			entity = new Drunkard(stats, inventory);
			break;

		case EnemyType::EVIL_MUMMY:
			inventory.weapon.range = 2;
			inventory.weapon.damage = 1;
			entity = new EvilMummy(stats, inventory);
			break;

		case EnemyType::UNKNOWN:
			entity = new Enemy();
			break;
		default:
			break;
		}
		break;

	case EntityType::NPC:
		entity = new NPC();
		break;

	case EntityType::ITEM:

		switch (i_type)
		{
		case ItemType::WEAPON:
			entity = new Weapon();
			break;
		case ItemType::ARMOR:
			entity = new Armor();
			break;
		case ItemType::ACCESSORY:
			break;
		case ItemType::UNKNOWN:
			entity = new Item();
			break;
		default:
			break;
		}
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
		if (item->data == entity) {
			entities.Del(item);
		}
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

void EntityManager::StartCombat(vector<Entity*> &enemies, Entity* summoner)
{
	currentStep = Fade_StepFade::TO_BLACKF;

	startCombat = true;

	combatManager = new CombatManager();

	int length = 0;

	combatManager->summoner = (Enemy*)summoner;
	combatManager->summoner->mainState = MainState::IN_COMBAT;
	combatManager->summoner->combatState = CombatState::WAITING;
	length++;

	for (int i = 0; i < app->sceneManager->currentScene->players.size(); i++)
	{
		combatManager->players.push_back((Player*)app->sceneManager->currentScene->players[i]);
		combatManager->players[i]->mainState = MainState::IN_COMBAT;
		combatManager->players[i]->combatState = CombatState::WAITING;
		combatManager->players[i]->stats.health = 3;
		combatManager->players[i]->ResetPath();
		length++;
	}

	combatManager->playersAlive = combatManager->players.size();

	for (int j = 0; j < enemies.size(); j++)
	{
		combatManager->enemies.Add((Enemy*)enemies[j]);
		combatManager->enemies[j]->mainState = MainState::IN_COMBAT;
		combatManager->enemies[j]->combatState = CombatState::WAITING;
		length++;
	}

	int paux = 0;
	int eaux = 0;

	for (size_t k = 0; k < length; k++)
	{
		if (k%2 == 0) {
			if (paux < combatManager->players.size()) { 
				combatManager->CombatList.push_back(combatManager->players[paux]);
				paux++;
			}
			else if (eaux <= combatManager->enemies.Count()) {
				combatManager->CombatList.push_back(combatManager->enemies[eaux]);
				eaux++;
			}
		}
		else {
			if (eaux <= combatManager->enemies.Count()) {
				if (k == 1) {
					combatManager->CombatList.push_back(combatManager->summoner);
				}
				else {
					combatManager->CombatList.push_back(combatManager->enemies[eaux]);
					eaux++;
				}
			}
			else if (paux < combatManager->players.size()) {
				combatManager->CombatList.push_back(combatManager->players[paux]);
				paux++;
			}
		}
	}

	combatManager->currentCharacterTurn = (Character*)combatManager->CombatList[0];

}

bool EntityManager::Update(float dt)
{

	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	if (!inCombat) {

		for (item = entities.start; item != NULL && ret == true; item = item->next)
		{
			pEntity = item->data;

			if (pEntity->active == false) continue;
			ret = item->data->Update(dt);
		}

	}
	else {
		combatManager->Update(dt);
	}

	return ret;
}

bool EntityManager::PostUpdate()
{

	if (combatFinished) {
		MakeEndCombatFade();
	}
	else if (startCombat) {
		MakeStartCombatFade();
	}

	if (combatFinished || startCombat) {
		float fadeRatio = (float)frameCount / (float)maxFadeFrames;
		// Render the black square with alpha on the screen
		SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
		SDL_RenderFillRect(app->render->renderer, &screenRect);
	}

	return true;
}

CombatManager::CombatManager()
{
}

CombatManager::~CombatManager()
{
}

bool CombatManager::Start()
{

	int windowW, windowH;
	windowW = app->entityManager->windowW;
	windowH = app->entityManager->windowH;

	turnArrow = app->tex->Load("Assets/Textures/TurnArrow.png");

	//tp all characters
	summoner->TpToCell(summoner->combatPos[0].x, summoner->combatPos[0].y);
	for (size_t i = 0; i < players.size(); i++)
	{
		players[i]->TpToCell(summoner->combatPos[i + 1].x, summoner->combatPos[i + 1].y);
		players[i]->ResetPath();
	}

	//prepare UI
	// 25px per letter
	SDL_Rect nextturnPos = { windowW * 0.3 - 350, (windowH * 0.8) - 75, 200,50 };
	nextTurnButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 0, "END TURN", nextturnPos, app->sceneManager->currentScene);
	nextTurnButton->animated = false;
	nextTurnButton->debug = true;

	SDL_Rect movePos = { windowW * 0.3 - 350, windowH * 0.8, 100,50 };
	moveButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "MOVE", movePos, app->sceneManager->currentScene);
	moveButton->animated = false;
	moveButton->debug = true;

	SDL_Rect attackPos = { windowW * 0.3 - 350, (windowH * 0.8) + 75, 150,50 };
	attackButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "ATTACK", attackPos, app->sceneManager->currentScene);
	attackButton->animated = false;
	attackButton->debug = true;

	//start fisrt turn
	CombatList[0]->combatState = CombatState::IDLE;
	startTime = SDL_GetTicks();

	UpdateNavigation();

	return true;
}

bool CombatManager::Update(float dt)
{

	Uint32 currentTime = SDL_GetTicks();
	Uint32 elapsedTime = currentTime - startTime;

	seconds = (elapsedTime / 1000) % 60;
	//Seconds = elapsedTime % 1000;

	if (currentCharacterTurn->combatState == CombatState::WAITING || seconds >= 60.0f)
	{
		currentCharacterTurn->combatState = CombatState::WAITING;
		currentCharacterTurn = CombatList[NextTurn()];
		currentCharacterTurn->combatState = CombatState::IDLE;
		PreapareUINextTurn();
		app->sceneManager->currentScene->cameraFocus = currentCharacterTurn;
	}

	for (int i = 0; i < CombatList.size(); i++)
	{
		CombatList[i]->Update(dt);
	}

	for (int i = 0; i < CombatList.size(); i++)
	{
		SDL_Rect posSize = { 10, i * CombatList[i]->idleAnim.frames[0].h / 2 + 20, CombatList[i]->idleAnim.frames[0].w / 2, CombatList[i]->idleAnim.frames[0].h / 2 };
		app->render->DrawTextureResize(CombatList[i]->texture, &posSize, &CombatList[i]->idleAnim.frames[0]);
		if (CombatList[i] == currentCharacterTurn) {
			posSize.x += CombatList[i]->idleAnim.frames[0].w / 2;
			posSize.y -= 10;
			app->render->DrawTextureResize(turnArrow, &posSize, NULL);
		}
	}

	CheckIfCharDead();

	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN || (enemies.Count() == 0 && summoner == nullptr) || playersAlive == 0)
	{
		app->entityManager->combatFinished = true;
		app->entityManager->currentStep = Fade_StepFade::TO_BLACKF;
	}

	return true;
}

bool CombatManager::CleanUp()
{
	return false;
}

void CombatManager::DestroyEntity(Character* entity, int i)
{
	if (entity == summoner) {
		app->sceneManager->currentScene->DeleteEnemy(summoner);
		app->entityManager->DestroyEntity(summoner);
		delete summoner;
		summoner = nullptr;
	}
	else {

		ListItem<Enemy*>* item;

		for (item = enemies.start; item != NULL; item = item->next)
		{
			if (item->data == entity) {
				item->data->CleanUp();
				enemies.Del(item);
				delete item->data;
			}
		}
	}

	CombatList.erase(CombatList.begin() + i);
}

int CombatManager::NextTurn()
{
	turn++;

	if (turn >= CombatList.size())
		turn = 0;

	startTime = SDL_GetTicks();

	return turn;
}

void CombatManager::EndCombat()
{
	//UI
	app->guiManager->DeleteGuiControl(nextTurnButton);
	app->guiManager->DeleteGuiControl(moveButton);
	app->guiManager->DeleteGuiControl(attackButton);
	app->guiManager->CleanUp();

	ClearNavigation();

	// victory
	for (int i = 0; i < players.size(); i++)
	{
		players[i]->mainState = MainState::OUT_OF_COMBAT;
		players[i]->combatState = CombatState::NONE;
		players[i]->exploringState = ExploringState::IDLE;
		players[i]->PosState = Direction::DR;
		if (i == 1) {
			players[i]->TpToCell(app->sceneManager->currentScene->GetPlayer()->GetTile().x - 1, app->sceneManager->currentScene->GetPlayer()->GetTile().y);
		}
		players[i]->stats.health = 3;
		players[i] = nullptr;
	}
	players.clear();

	ListItem<Enemy*>* item;

	for (item = enemies.start; item != NULL; item = item->next)
	{
		item->data->CleanUp();
		enemies.Del(item);
		delete item->data;
	}
	enemies.Clear();

	if (summoner != nullptr) {
		summoner->CleanUp();
		app->sceneManager->currentScene->DeleteEnemy(summoner);
		app->entityManager->DestroyEntity(summoner);
		delete summoner;
	}

	currentCharacterTurn = nullptr;

	CombatList.clear();

	app->tex->UnLoad(turnArrow);

	app->sceneManager->currentScene->cameraFocus = app->sceneManager->currentScene->GetPlayer();
}

void CombatManager::CheckIfHit(iPoint& dest, Weapon* weapon)
{
	for (int i = 0; i < CombatList.size(); i++) {
		if (CombatList[i]->GetTile() == dest) {
			CombatList[i]->stats.health -= weapon->damage;
			if (CombatList[i]->stats.health <= 0) {
				CombatList[i]->combatState = CombatState::DEAD;
			}
		}
	}
}

Player* CombatManager::GetClosestPlayer(Character* entity, int& dist)
{

	dist = 999;
	int	aux;
	int iterator = -1;

	for (size_t i = 0; i < players.size(); i++)
	{
		if (players[i]->combatState != CombatState::DEAD) {
			aux = abs(abs(players[i]->GetTile().x) - abs(entity->GetTile().x)) + abs(abs(players[i]->GetTile().y) - abs(entity->GetTile().y));
			if (aux < dist) {
				dist = aux;
				iterator = i;
			}
		}
	}

	if (iterator == -1) {
		entity->combatState = CombatState::NONE;
		return nullptr;
	}

	return players[iterator];
}

void CombatManager::CheckIfCharDead()
{
	for (int i = 0; i < CombatList.size(); i++) {
		if (CombatList[i]->combatState == CombatState::DEAD) {
			for (size_t j = 0; j < CombatList.size(); j++)
			{
				if (currentCharacterTurn == CombatList[j]) {
					if (i < j) {
						turn--;
					}
				}
			}
			if (CombatList[i]->type == EntityType::PLAYER) {
				CombatList.erase(CombatList.begin() + i);
				playersAlive--;
			}
			else {
				DestroyEntity(CombatList[i], i);
			}
			i--;
			if (i < 0) {
				i = 0;
			}
		}
	}
}

void CombatManager::UIEvent(int id)
{
	if (currentCharacterTurn->combatState != CombatState::MOVING) {
		if (id == 0) {
			currentCharacterTurn->combatState = CombatState::WAITING;
			currentCharacterTurn = CombatList[NextTurn()];
			currentCharacterTurn->combatState = CombatState::IDLE;
			PreapareUINextTurn();
		}
		if (id == 1) {
			if (currentCharacterTurn->combatState == CombatState::ATTACKING) {
				currentCharacterTurn->combatState = CombatState::IDLE;
				dynamic_cast<Player*>(currentCharacterTurn)->rangeTiles.Clear();
			}
			else {
				ClearNavigation();
				currentCharacterTurn->combatState = CombatState::ATTACKING;
				dynamic_cast<Player*>(currentCharacterTurn)->rangeTiles.Clear();
				dynamic_cast<Player*>(currentCharacterTurn)->FindRange(currentCharacterTurn->inventory.weapon.range);
			}
		}
		if (id == 2) {
			if (currentCharacterTurn->combatState == CombatState::MOVEMENT) {
				currentCharacterTurn->combatState = CombatState::IDLE;
				dynamic_cast<Player*>(currentCharacterTurn)->rangeTiles.Clear();
			}
			else {
				UpdateNavigation();
				currentCharacterTurn->combatState = CombatState::MOVEMENT;
				dynamic_cast<Player*>(currentCharacterTurn)->rangeTiles.Clear();
				dynamic_cast<Player*>(currentCharacterTurn)->FindRange(currentCharacterTurn->stats.movement - currentCharacterTurn->movementUsed);
			}
		}
		if (id == 3) {

		}
		if (id == 4) {

		}
	}
	app->input->ResetMouseButtonState();
}

void CombatManager::PreapareUINextTurn()
{
	ClearNavigation();
	if (currentCharacterTurn->type != EntityType::PLAYER) {
		nextTurnButton->active = false;
		attackButton->active = false;
		moveButton->active = false;
	}
	else {
		nextTurnButton->active = true;
		attackButton->active = true;
		moveButton->active = true;
	}
}

void CombatManager::UpdateNavigation(Entity* pFocus)
{
	for (size_t i = 0; i < CombatList.size(); i++)
	{
		if (CombatList[i] != currentCharacterTurn && pFocus != CombatList[i]) {
			pathfindingBlock.push_back((CombatList[i]->GetTile().y * app->map->GetMapTilesWidth()) + CombatList[i]->GetTile().x);
			int aux = pathfindingBlock.back();
			app->map->pathfinding->map[aux] = 0;
		}
	}
}

void CombatManager::ClearNavigation()
{
	for (size_t i = 0; i < pathfindingBlock.size(); i++)
	{
		app->map->pathfinding->map[pathfindingBlock[i]] = 1;
	}
	pathfindingBlock.clear();
}

void EntityManager::MakeStartCombatFade()
{
	if (currentStep == Fade_StepFade::TO_BLACKF)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			currentStep = Fade_StepFade::FROM_BLACKF;
			combatManager->Start();
			app->audio->PlayMusic(config.attribute("audio").as_string(), 0);
			inCombat = true;
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			startCombat = false;
			currentStep = Fade_StepFade::NOF;
		}
	}
}

void EntityManager::MakeEndCombatFade()
{
	if (currentStep == Fade_StepFade::TO_BLACKF)
	{
		++frameCount;
		if (frameCount >= maxFadeFrames)
		{
			currentStep = Fade_StepFade::FROM_BLACKF;
			combatManager->EndCombat();
			delete combatManager;
			combatManager = nullptr;
			inCombat = false;
			app->audio->PlayMusic(app->sceneManager->currentScene->sceneconfig.attribute("audio").as_string(), 0);
		}
	}
	else
	{
		--frameCount;
		if (frameCount <= 0)
		{
			currentStep = Fade_StepFade::NOF;
			combatFinished = false;
		}
	}
}
