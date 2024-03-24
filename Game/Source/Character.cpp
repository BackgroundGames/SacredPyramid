#include "Character.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Map.h"


Character::Character() : Entity(EntityType::UNKNOWN)
{
	name.Create("Character");
}

Character::~Character() {}

bool Character::Awake()
{
	return true;
}

bool Character::Start()
{
	//Initialize Character parameters

	if (texture == nullptr) {
		texture = app->tex->Load(parameters.attribute("texturePath").as_string());
		app->tex->GetSize(texture, texW, texH);
	}

	TpToCell(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	selectionTex = app->render->GetSelectionTex();

	PosState = Direction::DR;
	mainState = MainState::NONE;
	combatState = CombatState::NONE;
	exploringState = ExploringState::NONE;

	return true;
}

bool Character::Update(float dt)
{
	//if player is requested to move, do movement till finish
	if (move)
	{
		DoPathMoving();

		// debug the path
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
	prect.w = texW;
	prect.h = texH;
	if (app->debug)
		app->render->DrawRectangle(prect, 255, 0, 0, 255, false);

	//darw
	//pos x en la imagen
	prect.x = 245;

	if (PosState == Direction::UL) {
		//pos y en la imagen
		prect.y = 116;
		app->render->IDrawTexture(texture, position.x, position.y - texH / 2, NULL);
	}																		 
	else if (PosState == Direction::UR) {									 
		prect.y = 116;														 
		app->render->DrawTexture(texture, position.x, position.y - texH / 2, NULL);
	}																		 
	else if (PosState == Direction::DL) {	
		prect.y = 216;
		app->render->IDrawTexture(texture, position.x, position.y - texH / 2, NULL);
	}																		 
	else if (PosState == Direction::DR) {									 
		prect.y = 216;														 
		app->render->DrawTexture(texture, position.x, position.y - texH / 2, NULL);
	}

	if (app->debug)
	{
		//debug the midle of the player rect
		prect.x = position.x + texW/2 -1;
		prect.y = position.y + texH/2 -1;
		prect.w = 3;
		prect.h = 3;
		app->render->DrawRectangle(prect, 255, 255, 255, 255, false);
	}

	return true;
}

bool Character::CleanUp()
{
	return true;
}

void Character::TpToCell(int x, int y)
{
	position = iPoint(app->map->MapToWorld(x, y));
	position += iPoint((app->map->GetTileWidth() / 2) - (texW / 2), (app->map->GetTileHeight()) - (texH / 2));
}

void Character::moveTo(iPoint destination)
{
	//check if dest is walkable
	if (app->map->pathfinding->CreatePath(GetTile(), destination) != -1)
	{
		path = app->map->pathfinding->GetLastPath();

		//check if dest is walkable
		if (path->Count() != 1) {
			move = true;
			pathingIteration = 0;
			prevDestination = destination;
		}
		else 
		{
			if (move) 
			{
				move = false;
				TpToCell(path->At(0)->x, path->At(0)->y);
			}
		}
	}
}

void Character::DoPathMoving()
{
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
	int PlayerMidPosX = position.x + texW / 2;

	//calculate movement direction
	if (NextPathX > PlayerMidPosX)
	{
		translationOffset.x += 2;

		if (NextPathy > (position.y + texH / 2))
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
	else if (NextPathX < PlayerMidPosX)
	{
		translationOffset.x -= 2;

		if (NextPathy > (position.y + texH / 2))
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
	if (abs(translationOffset.x) >= app->map->GetTileWidth() / 2)
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

iPoint Character::GetTile()
{
	//el 14 seria la meitat del w i el 31 la meitat de h
	return app->map->WorldToMap(position.x + texW/2 - app->map->GetTileWidth() / 2, position.y + texH/2 - app->map->GetTileHeight()/2);
}
