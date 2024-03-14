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

Player::~Player() {

}

bool Player::Awake() {

	return true;
}

bool Player::Start() {
	
	Character::Start();

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


	//If space button is pressed modify put player in the cell of the cursor
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		Character::TpToCell(mouseTile.x, mouseTile.y);

	//move to the tile clicked
	if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (mouseTile != prevDestination)
			Character::moveTo(mouseTile);
	}

	Character::Update(dt);

	return true;
}

bool Player::CleanUp()
{
	return true;
}
