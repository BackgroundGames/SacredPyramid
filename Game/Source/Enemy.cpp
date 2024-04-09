#include "Enemy.h"
#include "App.h"
#include "Map.h"
#include "Render.h"
#include "Player.h"
#include "Log.h"
#include "Pathfinding.h"
#include "Bandit.h"
#include "Drunkard.h"
#include "Textures.h"

Enemy::Enemy()
{
	name.Create("Enemy");
	type = EntityType::ENEMY;
}

Enemy::Enemy(uint hp, uint mp, uint atk, uint def, uint matk, uint mdef, uint ini)
{
	name.Create("Enemy");
	type = EntityType::ENEMY;
	this->stats.health = hp;
	this->stats.mana = mp;
	this->stats.attack = atk;
	this->stats.defense = def;
	this->stats.magicAttack = matk;
	this->stats.magicDefense = mdef;
	this->stats.initiative = ini;
}

Enemy::~Enemy() {}

bool Enemy::Awake()
{
	return true;
}

bool Enemy::Start()
{
	Character::Start();

	currentAnimation = &idleAnim;
	texW = currentAnimation->GetCurrentFrame().w;
	texH = currentAnimation->GetCurrentFrame().h;

	TpToCell(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	mainState = MainState::OUT_OF_COMBAT;
	combatState = CombatState::NONE;
	exploringState = ExploringState::IDLE;

	observer = app->sceneManager->currentScene->GetPlayer();

	return true;
}

bool Enemy::Update(float dt)
{
	distanceFromPlayer = DistanceToTile(GetTile(), app->sceneManager->currentScene->GetPlayer()->GetTile());

	if (app->sceneManager->currentScene->settings) {
		previousEState = exploringState;
		exploringState = ExploringState::NONE;
	}

	switch (mainState)
	{
	case MainState::OUT_OF_COMBAT:
		switch (exploringState)
		{
		case ExploringState::IDLE:

			currentAnimation = &idleAnim;

			/*if (app->sceneManager->currentScene->GetPlayer()->GetTile() != prevDestination) {
				if (moveTo(app->sceneManager->currentScene->GetPlayer()->GetTile())) {
					exploringState = ExploringState::MOVING;
				}
			}*/

			if (GetMouseTile(mousePos) == GetTile()) 
			{
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && app->sceneManager->currentScene->GetPlayer()->interacted != this)
				{
					NotifyObserver();
				}
			}

			if (assaulted) {

				for (pugi::xml_node enemyNode = parameters.child("minion"); enemyNode; enemyNode = enemyNode.next_sibling("minion"))
				{
					Enemy* enemy;
					if (enemyNode.attribute("id").as_int() == 0) {
						enemy = new Bandit();
					}
					else {
						enemy = new Drunkard();
					}

					enemy->id = enemyNode.attribute("id").as_uint();
					enemies.push_back(enemy);
					enemy->parameters = enemyNode;
					enemy->Start();
					enemy->mainState = MainState::NONE;
					enemy->combatState = CombatState::NONE;
					enemy->exploringState = ExploringState::NONE;
				}
				app->entityManager->StartCombat(enemies, this);
				assaulted = false;
			}

			break;
		case ExploringState::MOVING:

			//move to the tile clicked
			if (app->sceneManager->currentScene->GetPlayer()->GetTile() != prevDestination) {
				moveTo(app->sceneManager->currentScene->GetPlayer()->GetTile());
			}

			DoPathMoving();

			if (!move) {
				exploringState = ExploringState::IDLE;
			}

			break;
		case ExploringState::NONE:
			if (!app->sceneManager->currentScene->settings) {
				exploringState = previousEState;
			}
			break;
		default:
			break;
		}
		break;
	case MainState::IN_COMBAT:
		switch (combatState)
		{
		case CombatState::WAITING:
			break;
		case CombatState::IDLE:
			if (distanceFromPlayer > 1) {
				app->map->pathfinding->CreatePath(GetTile(), app->sceneManager->currentScene->GetPlayer()->GetTile());
				if (distanceFromPlayer > 3) {
					moveTo(*app->map->pathfinding->GetLastPath()->At(3));
				}
				else {
					moveTo(*app->map->pathfinding->GetLastPath()->At(distanceFromPlayer - 1));
				}
				combatState = CombatState::MOVING;
			}
			else {
				combatState = CombatState::WAITING;
			}
			break;
		case CombatState::MOVING:

			if (move) {
				DoPathMoving();
			}
			else {
				combatState = CombatState::WAITING;
			}

			break;
		case CombatState::ATTACKING:
			break;
		case CombatState::DEAD:
			break;
		case CombatState::NONE:
			break;
		default:
			break;
		}
		break;
	case MainState::NONE:
		break;
	default:
		break;
	}

	Character::Update(dt);

	return true;
}

bool Enemy::CleanUp()
{
	enemies.clear();
	Character::CleanUp();
	return true;
}