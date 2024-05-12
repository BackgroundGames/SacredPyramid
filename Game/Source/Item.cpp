#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Map.h"
#include "Log.h"
#include "Point.h"

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
	type = EntityType::ITEM;
	Awake();
}

Item::~Item() {}

bool Item::Awake() 
{
	return true;
}

bool Item::Start() 
{
	app->tex->GetSize(texture, texW, texH);
	
	PlaceItem(parameters.attribute("x").as_int(), parameters.attribute("y").as_int());

	return true;
}

bool Item::Update(float dt)
{
	app->render->DrawTexture(texture, position.x, position.y - texH / 2);

	return true;
}

bool Item::CleanUp()
{
	app->tex->UnLoad(texture);
	texture = nullptr;
	return true;
}

void Item::PlaceItem(int x, int y)
{
	position = iPoint(app->map->MapToWorld(x, y));
	position += iPoint((app->map->GetTileWidth() / 2) - (texW / 2), (app->map->GetTileHeight()) - (texH / 2));
}
