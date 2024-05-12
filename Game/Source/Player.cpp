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
#include "InventoryMenu.h"
#include <queue>

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

	ResetPath();
	interacted = nullptr;

	movementUsed = 0;
	maxHealth = 3;

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

			ResetPath();

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
				if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && move == false && app->debug) {
					TpToCell(destination.x, destination.y);
					app->sceneManager->currentScene->LockCamera();
				}
			}

			if (app->input->GetKey(SDL_SCANCODE_I) == KEY_DOWN && app->sceneManager->currentScene->settings == false && app->sceneManager->currentScene->inventory == false)
			{
				app->sceneManager->OpenInventory();
				previousState = exploringState;
				exploringState = ExploringState::NONE;
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
				exploringState = ExploringState::IDLE;
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

		if (app->entityManager->inCombat) {
			DrawLife();
		}

		switch (combatState)
		{

		case CombatState::WAITING:
			movementUsed = 0;
			rangeTiles.Clear();
			move = false;
			path.Clear();
			break;

		case CombatState::IDLE:

			app->audio->StopFx(sandChannel);

			if (PosState == Direction::UL || PosState == Direction::UR) {
				currentAnimation = &idleAnimB;
			}
			else {
				currentAnimation = &idleAnim;
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
			}
			break;

		case CombatState::ATTACKING:

			if (DistanceToTile(GetTile(), destination) <= (inventory.weapon.range)) {
				iPoint highlightedTileWorld = app->map->MapToWorld(destination.x, destination.y);
				app->render->DrawTexture(selectionTex, highlightedTileWorld.x, highlightedTileWorld.y + app->map->GetTileHeight() / 2);
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
					app->audio->PlayFx(atack);
					app->entityManager->combatManager->CheckIfHit(destination, &inventory.weapon);
					combatState = CombatState::IDLE;
					rangeTiles.Clear();
					app->input->ResetMouseButtonState();
				}
			}

			for (size_t i = 1; i < rangeTiles.Count(); i++)
			{
				iPoint highlightedTileWorld = iPoint(rangeTiles[i].x, rangeTiles[i].y);
				if (app->map->pathfinding->IsWalkable(highlightedTileWorld)) {
					highlightedTileWorld = app->map->MapToWorld(rangeTiles[i].x, rangeTiles[i].y);
					app->render->DrawTexture(selectionTex, highlightedTileWorld.x, highlightedTileWorld.y + app->map->GetTileHeight() / 2);
				}
			}

			break;
		case CombatState::DEAD:
			break;
		case CombatState::MOVEMENT:

			if (DistanceToTile(GetTile(), destination) <= (stats.movement - movementUsed)) {
				if (app->map->pathfinding->CreatePath(GetTile(), destination) != -1 && app->map->pathfinding->GetLastPath()->Count() - 1 <= (stats.movement - movementUsed)) {
					path.Clear();
					for (size_t i = 1; i < app->map->pathfinding->GetLastPath()->Count() && i <= (stats.movement - movementUsed); i++)
					{
						path.PushBack(*app->map->pathfinding->GetLastPath()->At(i));
					}
					DebugPath();
					if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
						if (moveTo(*path.At(path.Count() -1))) {
							combatState = CombatState::MOVING;
							movementUsed += path.Count() - 1;
							rangeTiles.Clear();
							app->input->ResetMouseButtonState();
						}
					}
				}
			}

			for (size_t i = 1; i < rangeTiles.Count(); i++)
			{
				iPoint highlightedTileWorld = iPoint(rangeTiles[i].x, rangeTiles[i].y);
				app->map->pathfinding->CreatePath(GetTile(), highlightedTileWorld);
				if (app->map->pathfinding->IsWalkable(highlightedTileWorld) && app->map->pathfinding->GetLastPath()->Count() - 1 <= (stats.movement - movementUsed)) {
					highlightedTileWorld = app->map->MapToWorld(rangeTiles[i].x, rangeTiles[i].y);
					app->render->DrawTexture(selectionTex, highlightedTileWorld.x, highlightedTileWorld.y + app->map->GetTileHeight() / 2);
				}
				else if(app->map->pathfinding->IsWalkable(highlightedTileWorld)) {
					highlightedTileWorld = app->map->MapToWorld(rangeTiles[i].x, rangeTiles[i].y);
					app->render->DrawTexture(selectionTexR, highlightedTileWorld.x, highlightedTileWorld.y + app->map->GetTileHeight() / 2);
				}
			}

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

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && app->sceneManager->currentScene->settings == false && app->sceneManager->currentScene->inventory == false) {
		app->sceneManager->OpenGamePause();
		previousState = exploringState;
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

void Player::FindRange(int range)
{
	vector<vector<bool>> visited(app->map->GetMapTilesWidth(), vector<bool>(app->map->GetMapTilesHeight(), false));
	queue<iPoint> q;

	q.push(GetTile());
	visited[GetTile().x][GetTile().y] = true;

	// Definir los movimientos posibles (arriba, abajo, izquierda, derecha)
	int dx[] = { 0, 0, -1, 1 };
	int dy[] = { 1, -1, 0, 0 };

	while (q.size() != 0) {
		iPoint currentTile = q.front();
		q.pop();

		// Agregar el tile actual a la lista de tiles de ataque
		rangeTiles.PushBack(currentTile);

		// Explorar los tiles adyacentes
		for (int i = 0; i < 4; ++i) {
			int newX = currentTile.x + dx[i];
			int newY = currentTile.y + dy[i];

			// Verificar si el nuevo tile está dentro del mapa y no ha sido visitado
			if (isValidTile(newX, newY) && !visited[newX][newY]) {
				// Verificar si el nuevo tile está dentro del alcance del ataque
				if (abs(newX - GetTile().x) + abs(newY - GetTile().y) <= range) {
					q.push({ newX, newY });
					visited[newX][newY] = true;
				}
			}
		}
	}
}

bool Player::isValidTile(int x, int y)
{
	return x >= 0 && x < app->map->GetMapTilesWidth() && y >= 0 && y < app->map->GetMapTilesHeight();
}

