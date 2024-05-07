#include "Character.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
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
	parametersAnim = app->configFile.child("config").child("animations").child(name.GetString());
	return true;
}

bool Character::Start()
{
	//Initialize Character parameters

	if (texture == nullptr) {
		texture = app->tex->Load(parametersAnim.attribute("texturePath").as_string());
	}

	selectionTex = app->render->GetSelectionTex();
	selectionTexR = app->tex->Load("Assets/Maps/tileSelectionIso128r.png");

	PosState = Direction::DR;

	sandFx = app->audio->LoadFx("Assets/Audio/Fx/sandwalk.wav");

	return true;
}

bool Character::Update(float dt)
{
	//if player is requested to move, do movement till finish
	if (app->debug) 
	{
		stats.health = maxHealth;
		if (move) {
			DebugPath();
		}
	}

	currentAnimation->Update();
	texW = currentAnimation->GetCurrentFrame().w;
	texH = currentAnimation->GetCurrentFrame().h;

	//debug player rect
	prect.x = position.x;
	prect.y = position.y;
	prect.w = texW;
	prect.h = texH;

	if (PosState == Direction::UL) {
		app->render->IDrawTexture(texture, position.x, position.y - texH / 2, &currentAnimation->GetCurrentFrame());
	}																		 
	else if (PosState == Direction::UR) {									 													 
		app->render->DrawTexture(texture, position.x, position.y - texH / 2, &currentAnimation->GetCurrentFrame());
	}																		 
	else if (PosState == Direction::DL) {	
		app->render->IDrawTexture(texture, position.x, position.y - texH / 2, &currentAnimation->GetCurrentFrame());
	}																		 
	else if (PosState == Direction::DR) {									 													 
		app->render->DrawTexture(texture, position.x, position.y - texH / 2, &currentAnimation->GetCurrentFrame());
	}

	if (app->debug)
	{
		//debug rect of the player
		prect.x = position.x;
		prect.y = position.y;
		app->render->DrawRectangle(prect, 255, 0, 0, 255, false);
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
	app->tex->UnLoad(texture);
	app->audio->UnloadFx(sandFx);
	texture = nullptr;

	return true;
}

void Character::TpToCell(int x, int y)
{
	position = iPoint(app->map->MapToWorld(x, y));
	position += iPoint((app->map->GetTileWidth() / 2) - (texW / 2), (app->map->GetTileHeight()) - (texH / 2));
}

bool Character::moveTo(iPoint destination)
{
	//check if dest is walkable
	if (app->map->pathfinding->CreatePath(GetTile(), destination) != -1)
	{		
		//check if dest is walkable
		if (app->map->pathfinding->GetLastPath()->Count() != 1) {

			path.Clear();
			move = true;
			pathingIteration = 0;
			prevDestination = destination;

			for (size_t i = 0; i < app->map->pathfinding->GetLastPath()->Count(); i++)
			{
				path.PushBack(*app->map->pathfinding->GetLastPath()->At(i));
			}

			return true;
		}
		else 
		{
			if (move) 
			{
				move = false;
				pathingIteration = 0;
				path.Clear();
				TpToCell(GetTile().x, GetTile().y);
				app->sceneManager->currentScene->LockCamera();
				translationOffset = { 0, 0 };
			}
		}
	}

	return false;
}

void Character::DoPathMoving()
{
	//start from the fisrt cell
	if (pathingIteration == 0) {
		if (translationOffset.x == 0) {
			TpToCell(path.At(pathingIteration)->x, path.At(pathingIteration)->y);
			pathingIteration++;
			auxPosition = position;
			translationOffset.x = 0;
			translationOffset.y = 0;
		}
	}

	//get next cell X and Y (window pos pixels)
	int NextPathX = app->map->MapToWorld(path.At(pathingIteration)->x, path.At(pathingIteration)->y).x + app->map->GetTileWidth() / 2;
	int NextPathy = app->map->MapToWorld(path.At(pathingIteration)->x, path.At(pathingIteration)->y).y + app->map->GetTileHeight();
	int PlayerMidPosX = position.x + texW / 2;

	//calculate movement direction
	if (NextPathX > PlayerMidPosX)
	{
		translationOffset.x += velocity*2;

		if (NextPathy > (position.y + texH / 2))
		{
			translationOffset.y += velocity;
			PosState = Direction::DR;
		}
		else
		{
			translationOffset.y -= velocity;
			PosState = Direction::UR;
		}
	}
	else if (NextPathX < PlayerMidPosX)
	{
		translationOffset.x -= velocity * 2;

		if (NextPathy > (position.y + texH / 2))
		{
			PosState = Direction::DL;
			translationOffset.y += velocity;
		}
		else
		{
			PosState = Direction::UL;
			translationOffset.y -= velocity;
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
		if (pathingIteration > path.Count() - 1)
		{
			pathingIteration = 0;
			move = false;
			auxPosition = position;
			translationOffset = { 0, 0 };
			path.Clear();
		}
		//reasuring player is in the correct position in the cell
		else
		{
			auxPosition = position;
			translationOffset = { 0, 0 };
		}
	}
}

void Character::DebugPath()
{
	for (uint i = 0; i < path.Count(); ++i)
	{
		iPoint pos = app->map->MapToWorld(path.At(i)->x, path.At(i)->y);

		app->render->DrawTexture(selectionTex, pos.x, pos.y + app->map->GetTileHeight() / 2);

		//drw the midle of the cell
		SDL_Rect prect;
		prect.x = app->map->MapToWorld(path.At(i)->x, path.At(i)->y).x - 1 + app->map->GetTileWidth() / 2;
		prect.y = app->map->MapToWorld(path.At(i)->x, path.At(i)->y).y - 1 + app->map->GetTileHeight();
		prect.w = 2;
		prect.h = 2;

		app->render->DrawRectangle(prect, 255, 255, 255, 255, false);
	}
}

void Character::ResetPath()
{
	move = false;
	pathingIteration = 0;
	path.Clear();
	translationOffset = { 0, 0 };
}

iPoint Character::GetTile()
{
	//el 14 seria la meitat del w i el 31 la meitat de h
	return app->map->WorldToMap(position.x + texW / 2 - app->map->GetTileWidth() / 2, position.y + texH / 2 - app->map->GetTileHeight() / 2);
}

iPoint Character::GetMouseTile(iPoint mousePos)
{
	app->input->GetMousePosition(mousePos.x, mousePos.y);
	return app->map->WorldToMap(mousePos.x - app->render->camera.x - app->map->GetTileWidth() / 2,
		mousePos.y - app->render->camera.y - app->map->GetTileHeight() / 2);
}

uint Character::DistanceToTile(iPoint Tile1, iPoint Tile2)
{
	uint ret;

	ret = abs(abs(Tile1.x) - abs(Tile2.x)) + abs(abs(Tile1.y) - abs(Tile2.y));

	return ret;
}

void Character::DrawLife()
{
	SDL_Rect quat = { position.x + currentAnimation->GetCurrentFrame().w/2 - 50 , position.y - currentAnimation->GetCurrentFrame().h/2 - 20, 100, 10 };
	app->render->DrawRectangle(quat, 255, 0, 0, 150, true, true);
	quat.w = (stats.health * 100) / maxHealth;
	app->render->DrawRectangle(quat, 255, 0, 0, 255, true, true);
}

Inventory::Inventory()
{
	this->weapon = Weapon();
	this->armor = Armor();
	this->accessory = Accessory();
	this->consumable = Consumable();
	//this->invenotryItem = {};
}
/*
Inventory::Inventory(Weapon w, Armor ar, Accessory ac, Consumable c, vector<Item*> i)
{
	this->weapon = w;
	this->armor = ar;
	this->accessory = ac;
	this->consumable = c;
	this->invenotryItem = i;
}
*/