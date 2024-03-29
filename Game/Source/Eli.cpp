#include "Eli.h"
#include "App.h"

Eli::Eli()
{
	name.Create("Eli");
	subtype = PlayerType::ELI;
}

Eli::Eli(Stats stats)
{
	name.Create("Eli");
	this->stats = stats;
	subtype = PlayerType::ELI;
}

Eli::~Eli() {}

bool Eli::Awake()
{
	Player::Awake();
	return true;
}

bool Eli::Start()
{
	Player::Start();
	return true;
}

bool Eli::Update(float dt)
{
	mouseTile = Player::GetMouseTile(mousePos);
	
	Direction auxState = app->sceneManager->currentScene->GetZhaak()->PosState;

	if (auxState == Direction::DR)
	{
		mouseTile.x--;
	}
	else if(auxState == Direction::UR)
	{
		mouseTile.y++;
	}
	else if (auxState == Direction::DL)
	{
		mouseTile.y--;
	}
	else if (auxState == Direction::UL)
	{
		mouseTile.x++;
	}

	Player::Update(dt);
	return true;
}

bool Eli::CleanUp()
{
	Player::CleanUp();
	return true;
}
