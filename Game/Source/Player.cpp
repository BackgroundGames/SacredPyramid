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
#include "NPC.h"
#include "Enemy.h"
#include "GamePause.h"

Player::Player()
{
	name.Create("Player");
	type = EntityType::PLAYER;
}

Player::~Player() {}

bool Player::Start() {
	
	Character::Start();

	currentAnimation = &idleAnim;
	texW = currentAnimation->GetCurrentFrame().w;
	texH = currentAnimation->GetCurrentFrame().h;

	TpToCell(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	mainState = MainState::OUT_OF_COMBAT;
	combatState = CombatState::NONE;
	exploringState = ExploringState::IDLE;

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

			if (PosState == Direction::UL || PosState == Direction::UR) {
				currentAnimation = &idleAnimB;
			}
			else {
				currentAnimation = &idleAnim;
			}

			if (this != app->sceneManager->currentScene->GetPlayer())
			{
				if (app->sceneManager->currentScene->GetPlayer()->exploringState == ExploringState::MOVING || app->sceneManager->currentScene->GetPlayer()->exploringState == ExploringState::INTERACT)
				{
					if(destination != app->sceneManager->currentScene->GetPlayer()->GetTile()){
						if (app->sceneManager->currentScene->GetPlayer()->path.Count() > 0) {
							if (moveTo(*app->sceneManager->currentScene->GetPlayer()->path.At(app->sceneManager->currentScene->GetPlayer()->path.Count() - 2))) {
								exploringState = ExploringState::FOLLOWING;
							}
						}
					}
					
				}

				PosState = app->sceneManager->currentScene->GetPlayer()->PosState;

			}
			else
			{
				//move to the tile clicked
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP)
				{
					if (destination != prevDestination)
						if (moveTo(destination)) {
							exploringState = ExploringState::MOVING;
							sandChannel = app->audio->PlayFx(sandFx, -1);
						}
				}

				//If space button is pressed modify put player in the cell of the cursor
				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && move == false) {
					TpToCell(destination.x, destination.y);
					app->sceneManager->currentScene->LockCamera();
				}
			}
			break;

		case ExploringState::MOVING:

			if (app->sceneManager->currentScene->eli == app->sceneManager->currentScene->GetPlayer()) {
				if (PosState == Direction::UL || PosState == Direction::UR) {
					currentAnimation = &idleAnimB;
				}
				else {
					currentAnimation = &idleAnim;
				}
			}
			else {
				if (PosState == Direction::UL || PosState == Direction::UR) {
					currentAnimation = &walkingAnim;
				}
				else {
					currentAnimation = &walkingAnim;
				}
			}

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
				app->audio->StopFx(sandChannel);
			}

			break;

		case ExploringState::FOLLOWING:

			if (PosState == Direction::UL || PosState == Direction::UR) {
				currentAnimation = &idleAnimB;
			}
			else {
				currentAnimation = &idleAnim;
			}

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

			if (app->sceneManager->currentScene->GetPlayer()->exploringState == ExploringState::TALKING) {
				move = false;
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
			app->audio->StopFx(sandChannel);
			break;

		case ExploringState::INTERACT:

			if (app->sceneManager->currentScene->eli == app->sceneManager->currentScene->GetPlayer()) {
				if (PosState == Direction::UL || PosState == Direction::UR) {
					currentAnimation = &idleAnimB;
				}
				else {
					currentAnimation = &idleAnim;
				}
			}
			else {
				if (PosState == Direction::UL || PosState == Direction::UR) {
					currentAnimation = &walkingAnim;
				}
				else {
					currentAnimation = &walkingAnim;
				}
			}

			switch (interacted->type)
			{
			case EntityType::NPC:

				if (pathingIteration < path.Count() - 1) {
					DoPathMoving();
				}
				else {
					exploringState = ExploringState::TALKING;
					dynamic_cast<NPC*>(interacted)->startTalking = true;
					move = false;
					app->audio->StopFx(sandChannel);
					if (PosState == Direction::UL || PosState == Direction::UR) {
						currentAnimation = &idleAnimB;
					}
					else {
						currentAnimation = &idleAnim;
					}
				}
				break;
			case EntityType::ENEMY:

				if (pathingIteration < path.Count() - 1) {
					DoPathMoving();
				}
				else {
					exploringState = ExploringState::TALKING;
					dynamic_cast<Enemy*>(interacted)->assaulted = true;
					move = false;
					app->audio->StopFx(sandChannel);
					if (PosState == Direction::UL || PosState == Direction::UR) {
						currentAnimation = &idleAnimB;
					}
					else {
						currentAnimation = &idleAnim;
					}
				}

				break;

			default:
				break;
			}
			break;

		case ExploringState::NONE:
			if (PosState == Direction::UL || PosState == Direction::UR) {
				currentAnimation = &idleAnimB;
			}
			else {
				currentAnimation = &idleAnim;
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
			movementUsed = 0;
			break;

		case CombatState::IDLE:

			app->audio->StopFx(sandChannel);

			if (PosState == Direction::UL || PosState == Direction::UR) {
				currentAnimation = &idleAnimB;
			}
			else {
				currentAnimation = &idleAnim;
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && (DistanceToTile(GetTile(), destination) <= (stats.movement - movementUsed)))
			{
				if (moveTo(destination)) {
					combatState = CombatState::MOVING;
					movementUsed += path.Count() - 1;
				}
			}

			if (app->input->GetKey(SDL_SCANCODE_K) == KEY_DOWN) {
				combatState = CombatState::ATTACKING;
			}

			break;
			
		case CombatState::MOVING:

			if (PosState == Direction::UL || PosState == Direction::UR) {
				currentAnimation = &idleAnimB;
			}
			else {
				currentAnimation = &idleAnim;
			}

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

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && (DistanceToTile(GetTile(), destination) <= (inventory.weapon.range)))
			{
				app->entityManager->combatManager->CheckIfHit(destination, &inventory.weapon);
				combatState = CombatState::IDLE;
				app->input->ResetMouseButtonState();
			}

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

	interacted = control;
	exploringState = ExploringState::INTERACT;

	switch (interacted->type)
	{
	case EntityType::NPC:
		if (dynamic_cast<NPC*>(interacted)->id == 3) {
			iPoint aux = dynamic_cast<Character*>(interacted)->GetTile();
			aux.x--;
			moveTo(aux);
		}
		else {
			moveTo(dynamic_cast<Character*>(interacted)->GetTile());
		}
		break;

	case EntityType::ENEMY:

		moveTo(dynamic_cast<Character*>(interacted)->GetTile());
		break;

	default:
		break;
	}

	return false;
}

