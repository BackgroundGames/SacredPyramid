#include "Enemy.h"
#include "App.h"
#include "Map.h"
#include "Render.h"
#include "Player.h"
#include "Log.h"
#include "Pathfinding.h"
#include "Bandit.h"
#include "Drunkard.h"
#include "EvilMummy.h"
#include "Textures.h"

Enemy::Enemy()
{
	name.Create("Enemy");
	type = EntityType::ENEMY;
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

	if (app->sceneManager->currentScene->settings) {
		previousState = exploringState;
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
					Enemy* enemy = nullptr;

					switch (enemyNode.attribute("id").as_int())
					{
					case 0:
						enemy = new Bandit();
						break;
					case 1:
						enemy = new Drunkard();
						break;
					case 2:
						enemy = new EvilMummy();
						break;
					default:
						break;
					}

					enemy->id = enemyNode.attribute("id").as_uint();
					enemies.push_back(enemy);
					enemy->parameters = enemyNode;
					enemy->Start();
					enemy->mainState = MainState::NONE;
					enemy->combatState = CombatState::NONE;
					enemy->exploringState = ExploringState::NONE;
				}

				combatPos.push_back(iPoint{ parameters.attribute("cx").as_int(), parameters.attribute("cy").as_int() });
				combatPos.push_back(iPoint{ parameters.attribute("p1x").as_int(), parameters.attribute("p1y").as_int() });
				combatPos.push_back(iPoint{ parameters.attribute("p2x").as_int(), parameters.attribute("p2y").as_int() });

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
				exploringState = previousState;
			}
			break;
		default:
			break;
		}
		break;
	case MainState::IN_COMBAT:

		if (app->entityManager->inCombat) {
			DrawLife();
		}

		pFocus = app->entityManager->combatManager->GetClosestPlayer(this, distanceFromPlayer);
		aviableMovement = stats.movement - movementUsed;

		switch (combatState)
		{
		case CombatState::WAITING:
			movementUsed = 0;
			hasAttacked = false;
			break;
		case CombatState::IDLE:

			if (PosState == Direction::UL || PosState == Direction::UR) {
				currentAnimation = &idleAnimB;
			}
			else {
				currentAnimation = &idleAnim;
			}
			
			if(distanceFromPlayer <= inventory.weapon.range && !hasAttacked) {
				combatState = CombatState::ATTACKING;
				hasAttacked = true;
			}
			else {
				app->entityManager->combatManager->UpdateNavigation(pFocus);
				app->map->pathfinding->CreatePath(GetTile(), pFocus->GetTile());
				if (distanceFromPlayer > (aviableMovement + inventory.weapon.range)) {
					moveTo(*app->map->pathfinding->GetLastPath()->At(aviableMovement));
				}
				else {
					moveTo(*app->map->pathfinding->GetLastPath()->At(distanceFromPlayer - inventory.weapon.range));
					movementUsed = distanceFromPlayer - inventory.weapon.range;
				}
				combatState = CombatState::MOVING;
			}

			break;
		case CombatState::MOVING:

			if (PosState == Direction::UL || PosState == Direction::UR) {
				currentAnimation = &idleAnimB;
			}
			else {
				currentAnimation = &idleAnim;
			}

			if (move) {
				DoPathMoving();
			}
			else {
				if (distanceFromPlayer == inventory.weapon.range) {
					combatState = CombatState::ATTACKING;
				}
				else {
					combatState = CombatState::WAITING;
				}
			}

			break;
		case CombatState::ATTACKING:
			app->entityManager->combatManager->CheckIfHit(pFocus->GetTile(), &inventory.weapon);
			combatState = CombatState::WAITING;
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