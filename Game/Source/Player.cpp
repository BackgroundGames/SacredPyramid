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

	if (currentAnimation == nullptr) {
		idleAnim.loop = true;
		idleAnim.speed = 1.0f;
		idleAnim.PushBack({ 0, 0, 56, 123 });
	}

	currentAnimation = &idleAnim;
	texW = currentAnimation->GetCurrentFrame().w;
	texH = currentAnimation->GetCurrentFrame().h;

	TpToCell(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

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

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && (DistanceToTile(GetTile(), destination) <= (stats.movement - movementUsed)))
			{
				if (moveTo(destination)) {
					combatState = CombatState::MOVING;
					movementUsed += path.Count() - 1;
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
				if (movementUsed == stats.movement) {
					combatState = CombatState::WAITING;
					movementUsed = 0;
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
	/*for (size_t i = 0; i < app->sceneManager->currentScene->players.Count(); i++)
	{
		Player* auxp = (Player*)app->sceneManager->currentScene->players[i];
		auxp->TpToCell(auxp->GetTile().x, auxp->GetTile().y);
		auxp->move = false;
		auxp->translationOffset = { 0, 0 };
	}*/

	if (control->type == EntityType::NPC)
	{
		interacted = (Character*) control;
		exploringState = ExploringState::TALKING;
	}

	return false;
}

