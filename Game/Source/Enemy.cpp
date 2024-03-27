#include "Enemy.h"
#include "App.h"
#include "Map.h"
#include "Render.h"
#include "Player.h"
#include "Log.h"

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

	//aquests valors son per el pj de prova
	//texW = 28;
	//texH = 62;

	TpToCell(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	//Patrol
	iPoint patrol1 = app->map->WorldToMap(500, 400);
	iPoint patrol2 = app->map->WorldToMap(600, 400);

	mainState = MainState::OUT_OF_COMBAT;
	combatState = CombatState::NONE;
	exploringState = ExploringState::IDLE;

	observer = app->sceneManager->currentScene->GetPlayer();

	return true;
}

bool Enemy::Update(float dt)
{

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

			if (app->sceneManager->currentScene->GetPlayer()->GetTile() != prevDestination) {
				if (moveTo(app->sceneManager->currentScene->GetPlayer()->GetTile())) {
					exploringState = ExploringState::MOVING;
				}
			}

			//int X;
			//int Y;
			//app->input->GetMousePosition(X, Y);
			//X += -app->render->camera.x - app->map->GetTileWidth() / 2;
			//Y += -app->render->camera.y - app->map->GetTileHeight() / 2;
			//if (app->map->WorldToMap(X, Y) == GetTile()) {
			//	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
			//		if (moveTo(app->sceneManager->currentScene->GetPlayer()->GetTile())) {
			//			exploringState = ExploringState::MOVING;
			//			/*List<Entity*> enemies;
			//			enemies.Add(this);
			//			app->entityManager->StartCombat(enemies);*/
			//		}
			//		NotifyObserver();
			//	}
			//}

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
		case ExploringState::TALKING:
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
	return true;
}