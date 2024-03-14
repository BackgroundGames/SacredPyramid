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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	return true;
}

bool Player::Start() {
	//Initialize Player parameters
	position = iPoint(config.attribute("x").as_int(), config.attribute("y").as_int());

	texture = app->tex->Load(config.attribute("texturePath").as_string());
	app->tex->GetSize(texture, texW, texH);

	selectionTex = app->tex->Load(config.attribute("selectionPath").as_string());

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

	//do path doing tp
	/*if (app->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
		DoPathTping();
	}*/

	iPoint mousePos;
	app->input->GetMousePosition(mousePos.x, mousePos.y);
	iPoint mouseTile = app->map->WorldToMap(mousePos.x - app->render->camera.x - app->map->GetTileWidth() / 2,
		mousePos.y - app->render->camera.y - app->map->GetTileHeight() / 2);

	//If space button is pressed modify put player in the cell of the cursor
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
		TpToCell(mouseTile.x, mouseTile.y);

	//move to the tile clicked
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (mouseTile != prevDestination)
			moveTo(mouseTile);
	}

	//if player is requested to move, do movement till finish
	if (move) 
	{
		DoPathMoving();

		// debug the path
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);

			if (app->debug)
				app->render->DrawTexture(selectionTex, pos.x, pos.y + app->map->GetTileHeight() / 2);
			 
			//drw the midle of the cell
			SDL_Rect prect;
			prect.x = app->map->MapToWorld(path->At(i)->x, path->At(i)->y).x - 1 + app->map->GetTileWidth() / 2;
			prect.y = app->map->MapToWorld(path->At(i)->x, path->At(i)->y).y - 1 + app->map->GetTileHeight();
			prect.w = 2;
			prect.h = 2;

			if (app->debug)
				app->render->DrawRectangle(prect, 255, 255, 255, 255, false);
		}
	}

	//debug player rect
	prect.x = position.x;
	prect.y = position.y;
	prect.w = 28;
	prect.h = 62;
	if (app->debug)
		app->render->DrawRectangle(prect, 255, 0, 0, 255, false);

	//darw
	prect.x = 245;

	if (PosState == Direction::UL) {
		
		prect.y = 116;
		app->render->DrawTexture(texture, position.x, position.y - 62 / 2, &prect);
	}
	else if (PosState == Direction::UR) {
		prect.y = 116;
		app->render->IDrawTexture(texture, position.x, position.y - 62 / 2, &prect);
	}
	else if (PosState == Direction::DL) {
		prect.y = 216;
		app->render->DrawTexture(texture, position.x, position.y - 62 / 2, &prect);
	}
	else if (PosState == Direction::DR) {
		prect.y = 216;
		app->render->IDrawTexture(texture, position.x, position.y - 62 / 2, &prect);
	}

	if (app->debug)
	{
		//debug the midle of the player rect
		prect.x = position.x + 14;
		prect.y = position.y + 31;
		prect.w = 2;
		prect.h = 2;
		app->render->DrawRectangle(prect, 255, 255, 255, 255, false);
	}

	return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::TpToCell(int x, int y)
{
	position = iPoint(app->map->MapToWorld(x, y));
	position += iPoint((app->map->GetTileWidth() / 2) - (28 / 2), 0);
}

void Player::moveTo(iPoint destination)
{
	//check if dest is walkable
	if (app->map->pathfinding->CreatePath(app->map->WorldToMap(position.x, position.y + 15), destination) != -1) {
		const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

		//check if dest is walkable
		if (path->Count() != 1) {
			move = true;
			pathingIteration = 0;
			prevDestination = destination;
		}
		else {
			if (move) {
				move = false;
				TpToCell(path->At(0)->x, path->At(0)->y);
			}
		}
	}
}

void Player::DoPathMoving()
{
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

	//start from the fisrt cell
	if (pathingIteration == 0) {
		if (translationOffset.x == 0) {
			TpToCell(path->At(pathingIteration)->x, path->At(pathingIteration)->y);
			pathingIteration++;
			auxPosition = position;
			translationOffset.x = 0;
			translationOffset.y = 0;
		}
	}

	//get next cell X and Y (window pos pixels)
	int NextPathX = app->map->MapToWorld(path->At(pathingIteration)->x, path->At(pathingIteration)->y).x + app->map->GetTileWidth() / 2;
	int NextPathy = app->map->MapToWorld(path->At(pathingIteration)->x, path->At(pathingIteration)->y).y + app->map->GetTileHeight();
	int PlayerMidPosX = position.x + 28 / 2;

	//calculate movement direction
	if (NextPathX > PlayerMidPosX) 
	{
		translationOffset.x += 2;

		if (NextPathy > (position.y + 62 / 2)) 
		{
			translationOffset.y += 1;
			PosState = Direction::DR;
		}
		else 
		{
			translationOffset.y -= 1;
			PosState = Direction::UR;
		}
	}
	else if(NextPathX < PlayerMidPosX) 
	{
		translationOffset.x -= 2;

		if (NextPathy > (position.y + 62 / 2)) 
		{
			PosState = Direction::DL;
			translationOffset.y += 1;
		}
		else 
		{
			PosState = Direction::UL;
			translationOffset.y -= 1;
		}
	}

	//moidifying player pos
	position.x = auxPosition.x + translationOffset.x;
	position.y = auxPosition.y + translationOffset.y;

	//once we get to the next cell
	if (abs(translationOffset.x) >= app->map->GetTileWidth()/2) 
	{
		pathingIteration++;

		//check if path have finished
		if (pathingIteration > path->Count() - 1) 
		{
			pathingIteration = 0;
			finishMoving = true;
			move = false;
			auxPosition = position;
			translationOffset.x = 0;
			translationOffset.y = 0;
		}
		//reasuring player is in the correct position in the cell
		else 
		{
			//TpToCell(path->At(pathingIteration -1)->x, path->At(pathingIteration-1)->y);
			auxPosition = position;
			translationOffset.x = 0;
			translationOffset.y = 0;
		}
	}
}

void Player::DoPathTping()
{
	const DynArray<iPoint>* path = app->map->pathfinding->GetLastPath();

	if (pathingIteration == 0)
		pathingIteration = 1;

	if (path->Count() > 1 && !finishMoving) 
	{
		TpToCell(path->At(pathingIteration)->x, path->At(pathingIteration)->y);

		pathingIteration++;

		if (pathingIteration > path->Count() - 1) 
		{
			pathingIteration = 0;
			finishMoving = true;
		}
	}
}

iPoint Player::GetTile()
{
	//el 14 seria la meitat del w i el 31 la meitat de h
	return app->map->WorldToMap(position.x + 14 - app->map->GetTileWidth() / 2, position.y + 31 - app->map->GetTileWidth() / 4);
}
