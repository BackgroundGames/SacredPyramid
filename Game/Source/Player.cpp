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
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
		position.x += -0.2*dt;

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
		position.x += 0.2*dt;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
		position.y += -0.2 * dt;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
		position.y += 0.2 * dt;

	iPoint mousePos;
	app->input->GetMousePosition(mousePos.x, mousePos.y);
	iPoint mouseTile = app->map->WorldToMap(mousePos.x - app->render->camera.x - app->map->GetTileWidth() / 2,
		mousePos.y - app->render->camera.y - app->map->GetTileHeight() / 2);

	switch (mainState)
	{
	case MainState::OUT_OF_COMBAT:
		switch (exploringState)
		{
		case ExploringState::IDLE:

			//move to the tile clicked
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
			{
				if (mouseTile != prevDestination)
					if (moveTo(mouseTile)) {
						exploringState = ExploringState::MOVING;
					}
			}

			//If space button is pressed modify put player in the cell of the cursor
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && move == false) {
				TpToCell(mouseTile.x, mouseTile.y);
				app->sceneManager->currentScene->LockCamera();
			}

			break;
		case ExploringState::MOVING:

			//move to the tile clicked
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
			{
				if (mouseTile != prevDestination)
					moveTo(mouseTile);
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
	return true;
}

bool Player::OnGuiMouseClickEvent(Entity* control)
{
	TpToCell(GetTile().x,GetTile().y);
	move = false;
	return false;
}

