#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Map.h"

Player::Player()
{
	name.Create("Player");
	type = EntityType::PLAYER;
}

Player::~Player() {}

bool Player::Awake() {

	return true;
}

bool Player::Start() {
	
	Character::Start();

	//aquests valors son per el pj de prova
	//texW = 25;
	//texH = 55;

	mainState = MainState::OUT_OF_COMBAT;
	combatState = CombatState::NONE;
	exploringState = ExploringState::IDLE;

	app->sceneManager->currentScene->LockCamera();

	return true;
}

bool Player::Update(float dt)
{
	//Render the player texture and modify the position of the player using WSAD keys and render the texture
	if (app->debug)
	{
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			position.x += -0.2 * dt;

		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			position.x += 0.2 * dt;

		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			position.y += -0.2 * dt;

		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			position.y += 0.2 * dt;
	}

	destination = Player::GetMouseTile(mousePos);

	switch (mainState)
	{
	case MainState::OUT_OF_COMBAT:
		switch (exploringState)
		{
		case ExploringState::IDLE:

			if (this != app->sceneManager->currentScene->GetPlayer())
			{
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && destination != app->sceneManager->currentScene->GetPlayer()->GetTile() && app->sceneManager->currentScene->GetPlayer()->path.Count() > 0)
				{
					if (moveTo(*app->sceneManager->currentScene->GetPlayer()->path.At(app->sceneManager->currentScene->GetPlayer()->path.Count() - 2))) {
						exploringState = ExploringState::FOLLOWING;
					}
				}
			}
			else
			{
				//move to the tile clicked
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
				{
					if (destination != prevDestination)
						if (moveTo(destination))
							exploringState = ExploringState::MOVING;
				}

				//If space button is pressed modify put player in the cell of the cursor
				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && move == false) {
					TpToCell(destination.x, destination.y);
					app->sceneManager->currentScene->LockCamera();
				}
			}
			break;

		case ExploringState::MOVING:

			//move to the tile clicked
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				if (destination != prevDestination)
					moveTo(destination);
			}

			if (move)
			{
				DoPathMoving();
			}
			else
			{
				exploringState = ExploringState::IDLE;
			}

			break;

		case ExploringState::FOLLOWING:

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				if (destination != app->sceneManager->currentScene->GetPlayer()->GetTile()) {
					moveTo(*app->sceneManager->currentScene->GetPlayer()->path.At(app->sceneManager->currentScene->GetPlayer()->path.Count() - 2));
				}
				else {
					TpToCell(GetTile().x, GetTile().y);
					move = false;
				}
			}

			if (move)
			{
				DoPathMoving();
			}
			else
			{
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

		switch (combatState)
		{

		case CombatState::WAITING:
			break;

		case CombatState::IDLE:

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && (DistanceToTile(GetTile(), destination) <= (stats.mobility - mobilityUsed)))
			{
				if (moveTo(destination)) {
					combatState = CombatState::MOVING;
					mobilityUsed += path.Count() - 1;
				}
			}
			break;
			
		case CombatState::MOVING:

			if (move)
			{
				DoPathMoving();
			}
			else
			{
				combatState = CombatState::IDLE;
				if (mobilityUsed == stats.mobility) {
					combatState = CombatState::WAITING;
					mobilityUsed = 0;
				}
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

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && app->sceneManager->currentScene->settings == false) {
		app->sceneManager->OpenGamePause();
		previousEState = exploringState;
		exploringState = ExploringState::NONE;
	}

	Character::Update(dt);

	return true;
}

bool Player::CleanUp()
{
	Character::CleanUp();
	return true;
}

bool Player::OnGuiMouseClickEvent(Entity* control)
{
	for (size_t i = 0; i < app->sceneManager->currentScene->players.Count(); i++)
	{
		Player* auxp = (Player*)app->sceneManager->currentScene->players[i];
		auxp->TpToCell(auxp->GetTile().x, auxp->GetTile().y);
		auxp->move = false;
		auxp->translationOffset = { 0, 0 };
	}
	return false;
}

iPoint Player::GetMouseTile(iPoint mousePos)
{
	app->input->GetMousePosition(mousePos.x, mousePos.y);
	return app->map->WorldToMap(mousePos.x - app->render->camera.x - app->map->GetTileWidth() / 2,
								mousePos.y - app->render->camera.y - app->map->GetTileHeight() / 2);
}

